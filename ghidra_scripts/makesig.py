#Generates a ZHL-ready signature.
#@author nosoop, REPENTOGON team
#@category _NEW_
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

import os
import re
import json
import collections
import ghidra.program.model.lang.OperandType as OperandType
import ghidra.program.model.lang.Register as Register
import ghidra.program.model.address.AddressSet as AddressSet
from ghidra.program.model.symbol import SymbolType
from ghidra.program.model.data import Pointer
from ghidra.program.database.data import ArrayDB

from ghidra.program.model.listing import Function
from ghidra.program.model.symbol import SymbolTable, SourceType
from ghidra.program.model.address import Address
from ghidra.util.exception import InvalidInputException
from ghidra.program.model.mem import MemoryAccessException
from ghidra.util.task import TaskMonitor
from jarray import zeros  # Import 'zeros' to create Java byte arrays


zhlfunctionspath = r'C:\Users\Juan\Documents\GitHub\IsaacZHL\libzhl\functions' #set up your zhl functions dir
countfuncfails = 0
countfuncmult = 0
countfuncsucksex = 0

MAKE_SIG_AT = collections.OrderedDict([
	('fn', 'Sig at the start of function'),
	('cursor', 'Sig at instruction at cursor'),
	('namespc', 'Parse entire class/namespc(zhl)'),
	('zhlcheckup', 'Import ZHL functions/namespaces'),
	('regclass', 'lurk lua refs (RegisterClasses) [UNFINISHED]')
])

BytePattern = collections.namedtuple('BytePattern', ['is_wildcard', 'byte'])

def __bytepattern_ida_str(self):
	# return an IDA-style binary search string
	return '{:02X}'.format(self.byte) if not self.is_wildcard else '?'

def __bytepattern_sig_str(self):
	# return a ZHL-style byte signature
	return '{:02X}'.format(self.byte).lower() if not self.is_wildcard else '??'

BytePattern.ida_str = __bytepattern_ida_str
BytePattern.sig_str = __bytepattern_sig_str

def dumpOperandInfo(ins, op):
	t = hex(ins.getOperandType(op))
	print('  ' + str(ins.getPrototype().getOperandValueMask(op)) + ' ' + str(t))
	
	# TODO if register
	for opobj in ins.getOpObjects(op):
		print('  - ' + str(opobj))

def shouldMaskOperand(ins, opIndex):
	"""
	Returns True if the given instruction operand mask should be masked in the signature.
	"""
	optype = ins.getOperandType(opIndex)
	# if any(reg.getName() == "EBP" for reg in filter(lambda op: isinstance(op, Register), ins.getOpObjects(opIndex))):
		# return False
	return optype & OperandType.DYNAMIC or optype & OperandType.ADDRESS

def getMaskedInstruction(ins):
	"""
	Returns a generator that outputs either a byte to match or None if the byte should be masked.
	"""
	# print(ins)
	
	# resulting mask should match the instruction length
	mask = [0] * ins.length
	
	proto = ins.getPrototype()
	# iterate over operands and mask bytes
	for op in range(proto.getNumOperands()):
		# dumpOperandInfo(ins, op)
		
		# TODO deal with partial byte masks
		if shouldMaskOperand(ins, op):
			mask = [ m | v & 0xFF for m, v in zip(mask, proto.getOperandValueMask(op).getBytes()) ]
	# print('  ' + str(mask))
	
	for m, b in zip(mask, ins.getBytes()):
		if m == 0xFF:
			# we only check for fully masked bytes at the moment
			yield BytePattern(is_wildcard = True, byte = None)
		else:
			yield BytePattern(byte = b & 0xFF, is_wildcard = False)

# removes trailing wilds from the sig
def cleanupWilds(byte_pattern):
	for byte in reversed(byte_pattern):
		if byte.is_wildcard is False:
			break
		del byte_pattern[-1]


def parsefuncdeftype(str):
	if str == "uint":
		return "uint32_t"
	elif (str == "string") or (str == "String"):
		return "std_string"
	return str
	
def processfunc(fn, start_at = MAKE_SIG_AT['fn'], min_length = 1, zhlonly = True):
	cm = currentProgram.getCodeManager()
	if start_at == MAKE_SIG_AT['fn']:
		ins = cm.getInstructionAt(fn.getEntryPoint())
	elif start_at == MAKE_SIG_AT['cursor']:
		try:
			# Ghidra 10.4 introduces an additional parameter 'usePrototypeLength'
			# it will throw on older versions, so fall back to the previous version
			ins = cm.getInstructionContaining(currentAddress, False)
		except TypeError:
			ins = cm.getInstructionContaining(currentAddress)
	
	if not ins:
		raise Exception("Could not find entry point to function")

	pattern = "" # contains pattern string (supports regular expressions)
	byte_pattern = [] # contains BytePattern instances
	
	# keep track of our matches
	matches = []
	match_limit = 128
	
	while True: #do it till its unique, even if it goes beyond the function, fuck it! # fm.getFunctionContaining(ins.getAddress()) == fn:
		for entry in getMaskedInstruction(ins):
			byte_pattern.append(entry)
			if entry.is_wildcard:
				pattern += '.'
			else:
				pattern += r'\x{:02x}'.format(entry.byte)
		
		expected_next = ins.getAddress().add(ins.length)
		ins = ins.getNext()
		
		if ins.getAddress() != expected_next:
			# add wildcards until we get to the next instruction
			for _ in range(ins.getAddress().subtract(expected_next)):
				byte_pattern.append(BytePattern(is_wildcard = True, byte = None))
				pattern += '.'
		
		if len(byte_pattern) < min_length:
			continue
		
		if 0 < len(matches) < match_limit:
			# we have all the remaining matches, start only searching those addresses
			match_set = AddressSet()
			for addr in matches:
				match_set.add(addr, addr.add(len(byte_pattern)))
			matches = findBytes(match_set, pattern, match_limit, 1)
		else:
			# the matches are sorted in ascending order, so the first match will be the start
			matches = findBytes(matches[0] if len(matches) else None, pattern, match_limit)
		
		if len(matches) < 2:
			break
	
	cleanupWilds(byte_pattern)
	if not zhlonly:
		print("Signature for", fn.getName())
		print(*(b.ida_str() for b in byte_pattern))
		print("".join(b.sig_str() for b in byte_pattern))
	if not len(matches) == 1:
		if not zhlonly:
			print('Signature matched', len(matches), 'locations:', *(matches))
			printerr("Could not find unique signature")
	else:
		if start_at == MAKE_SIG_AT['fn']:
			if not zhlonly:
				print("---ZHL func prototype START---")
				print("")
			# parse params
			paramstring = ""
			for param in fn.getParameters():
				if param.getName() != "this":
					if len(paramstring) > 1:
						paramstring = paramstring + ", " 
					paramstring = paramstring + parsefuncdeftype(param.getFormalDataType().getDisplayName()) + " " + param.getName()
			# parse params end
			print('"'+"".join(b.sig_str() for b in byte_pattern)+'":')
			print(fn.getCallingConventionName() + " " + parsefuncdeftype(fn.getReturnType().getDisplayName()) + " " + fn.getParentNamespace().getName() + "::" + fn.getName() + "(" + paramstring + ");")
			print("")
			if not zhlonly:
				print("---ZHL func prototype END---")

def isStructureDataType(dataType):
    return dataType.getDataTypeClass() == DataTypeClass.STRUCTURE

def isPointerDataType(dataType):
    return isinstance(dataType, Pointer)

def processdatatype(nmspc):
	dtman = currentProgram.getDataTypeManager()
	structures = dtman.getAllStructures()
	for structure in structures:
		if structure.getName() == nmspc.getName():
			depends = ""
			print("")
			#for idx in range(structure.getNumComponents()): #iterating these twice because im lazy(its pretty damn fast anyway, the slow thing is getting the sigs)
			#	cmp = structure.getComponent(idx)
			#	if cmp.getFieldName():
			#		cmp = structure.getComponent(idx)
			#		if depends == "":
			#			depends = " depends ("
			#		else:
			#			depends = depends + ","
			#		
			#		if isinstance(cmp.getDataType(), ArrayDB):
			#			depends = depends + " " + cmp.getDataType().getName()
			#		elif isPointerDataType(cmp.getDataType()) or isStructureDataType(cmp.getDataType()):
			#			depends = depends + " " + cmp.getDataType().getName()
			if not (depends == ""):
				depends = depends + ")"
			print("")			
			print("struct " + nmspc.getName() + depends + " {")
			idx = 0
			for idx in range(structure.getNumComponents()):
				cmp = structure.getComponent(idx)
				if cmp.getFieldName():
					print("	" + parsefuncdeftype(cmp.getDataType().getName()) + " " + cmp.getFieldName() + " : " + hex(cmp.getOffset()) + ";")
				idx = idx + 1
			print("} : " + hex(structure.getLength()) + ";")
			return

def process(start_at=MAKE_SIG_AT['fn'], min_length=1):
	fm = currentProgram.getFunctionManager()
	fn = fm.getFunctionContaining(currentAddress)
	
	if start_at == MAKE_SIG_AT['regclass']:
		process_registerclasses(fn)
	
	elif start_at == MAKE_SIG_AT['namespc']:
		nmspc = fn.getParentNamespace()
		symtab = currentProgram.getSymbolTable()
		functions = symtab.getSymbols(nmspc) #currentProgram.getFunctionManager().getFunctions(True)
		for fn in functions:
			if fn.getSymbolType().toString() == "Function": #fn.getParentNamespace() == nmspc:
				fn = fm.getReferencedFunction(fn.getAddress())
				processfunc(fn)
		processdatatype(nmspc)
	else:
		processfunc(fn, start_at, min_length, False)



#registerclasses shit
def get_string_at_address(address):
    """Returns the string at a given address."""
    data = getDataAt(address)
    if data and data.hasStringValue():
        return data.getValue()
    return None

def get_function_at_address(address):
    """Returns the function at a given address."""
    function_manager = currentProgram.getFunctionManager()
    return function_manager.getFunctionContaining(address)

def generate_signature(function):
    """Generates a unique signature for a given function."""
    instructions = []
    instruction = getInstructionAt(function.getEntryPoint())
    while instruction and function.getBody().contains(instruction.getAddress()):
        instructions.append(instruction)
        instruction = instruction.getNext()

    signature = []
    for ins in instructions:
        for entry in getMaskedInstruction(ins):
            if entry.is_wildcard:
                signature.append('??')
            else:
                signature.append('{:02x}'.format(entry.byte))
    
    return ' '.join(signature)

def process_registerclasses(fn):
    """Processes a function to pair strings with their related function calls."""
    instruction = getInstructionAt(fn.getEntryPoint())

    while instruction and fn.getBody().contains(instruction.getAddress()):
        if instruction.getMnemonicString() == "MOV" and instruction.getNumOperands() > 1:
			#string followed by function
            if instruction.getOperandType(1) & OperandType.ADDRESS:
                string_address = instruction.getAddress(1)
                if string_address:
                    string_value = get_string_at_address(string_address)
                    if string_value:
                        next_instruction = instruction.getNext()
                        if next_instruction and next_instruction.getMnemonicString() == "PUSH":
                            func_address = next_instruction.getAddress(0)
                            if func_address:
                                func = get_function_at_address(func_address)
                                if func:
                                    signature = generate_signature(func)
                                    processfunc(func, MAKE_SIG_AT['fn'], 1, True)
                                    print("{} -> {} \n".format(string_value, signature, func.getName()))
        instruction = instruction.getNext()
	
	instruction = getInstructionAt(fn.getEntryPoint())
	while instruction and fn.getBody().contains(instruction.getAddress()):
		if instruction.getMnemonicString() == "PUSH" and instruction.getNumOperands() > 0:
				#function followed by string
				func_address = instruction.getAddress(0)
				if func_address:
					func = get_function_at_address(func_address)
					if func:
						next_instruction = instruction.getNext()
						if next_instruction and next_instruction.getMnemonicString() == "PUSH":
							string_address = next_instruction.getAddress(0)
							if string_address:
								string_value = get_string_at_address(string_address)
								if string_value:
									signature = generate_signature(func)
									processfunc(func, MAKE_SIG_AT['fn'], 1, True)
									print("{} -> {} \n".format(string_value, signature, func.getName()))
		instruction = instruction.getNext()

    #return results

#registerclasses shit


#zhl func registering shenanigans
#magical regex that grabs all the shit
patterns = {
    'function_signature': re.compile(r'"([0-9a-fA-F?]+)":\s*(?:(__\w+)\s+)?([\w*]+)\s+([\w*]+)::([\w*]+)\(([^)]*)\);') 
}

currfuncparams = 0

def twos_complement_array_to_decimal(arr):
    """
    Convert an array of signed integers to their unsigned equivalents based on 2's complement.

    :param arr: List of signed integers.
    :param bit_width: The number of bits in the 2's complement representation (e.g., 8 for bytes).
    :return: A list of unsigned integers.
    """
    result = []
    max_value = 1 << 8  # 2^bit_width

    for value in arr:
        if value < 0:
            # Convert the signed integer to an unsigned equivalent
            value = max_value + value
        result.append(value)

    return result


def find_function_address(signature):
    """
    Searches for a function by matching its byte signature at function entry points.
    """
    # Convert the signature from string format to a list of bytes
    byte_list = []
    for i in range(0, len(signature), 2):
        byte_str = signature[i:i+2]
        if byte_str == '??':
            byte_list.append(-1)  # Use -1 to represent wildcard
        else:
            try:
                byte_list.append(int(byte_str, 16))
            except ValueError:
                print("Invalid byte string:", byte_str)
                return None

    # Get the current program, function manager, and memory
    program = currentProgram
    function_manager = program.getFunctionManager()
    memory = program.getMemory()

    #print("Starting signature search for:", signature)
    #print("Parsed byte list (with wildcards):", byte_list)

    # Iterate over functions in the current program
    for function in function_manager.getFunctionsNoStubs(True):
        entry_point = function.getEntryPoint()
		
        try:
            # Read only the number of bytes needed for the signature
            func_bytes =  zeros(len(byte_list), 'b')  # Create a buffer for the bytes
            bytes_read = memory.getBytes(entry_point, func_bytes)
            func_bytes = twos_complement_array_to_decimal(func_bytes) #fuck you ghidra, fuck you!

            if bytes_read != len(byte_list):
                print("Error reading memory: not enough bytes at", entry_point)
                continue

            #print("Function: ", function.getName() ," at address:", entry_point, "Function bytes:", list(func_bytes))
            # Compare the function's entry bytes with the signature
            if matches_signature(func_bytes, byte_list):
                if not function.getName().startswith("FUN_"):
                   print(function.getName(), " already identified!")
                   return signature
                #print("Signature matched at function starting at:", entry_point)
                return entry_point  # Return the function's entry point address
        except MemoryAccessException as e:
            print("Error reading memory at address:", entry_point, "-", str(e))
            continue  # Skip if there's an error reading memory

    #print("!!!No matching function found for signature:", signature)
    return signature  # Return None if no function is found
	
def count_function_address(signature):
    """
    Counts for a function by matching its byte signature at function entry points.
    """
    # Convert the signature from string format to a list of bytes
    byte_list = []
    for i in range(0, len(signature), 2):
        byte_str = signature[i:i+2]
        if byte_str == '??':
            byte_list.append(-1)  # Use -1 to represent wildcard
        else:
            try:
                byte_list.append(int(byte_str, 16))
            except ValueError:
                print("Invalid byte string:", byte_str)
                return None

    # Get the current program, function manager, and memory
    program = currentProgram
    function_manager = program.getFunctionManager()
    memory = program.getMemory()
    count = 0

    #print("Starting signature search for:", signature)
    #print("Parsed byte list (with wildcards):", byte_list)

    # Iterate over functions in the current program
    for function in function_manager.getFunctionsNoStubs(True):
        entry_point = function.getEntryPoint()

        try:
            # Read only the number of bytes needed for the signature
            func_bytes =  zeros(len(byte_list), 'b')  # Create a buffer for the bytes
            bytes_read = memory.getBytes(entry_point, func_bytes)
            func_bytes = twos_complement_array_to_decimal(func_bytes) #fuck you ghidra, fuck you!

            if bytes_read != len(byte_list):
                print("Error reading memory: not enough bytes at", entry_point)
                continue

            #print("Function: ", function.getName() ," at address:", entry_point, "Function bytes:", list(func_bytes))
            # Compare the function's entry bytes with the signature
            if matches_signature(func_bytes, byte_list):
                #print("Signature matched at function starting at:", entry_point)
                count = count +1
                #return entry_point  # Return the function's entry point address
        except MemoryAccessException as e:
            print("Error reading memory at address:", entry_point, "-", str(e))
            continue  # Skip if there's an error reading memory

    #print("!!!No matching function found for signature:", signature)
    return count  # Return None if no function is found


def find_closest_function_address(signature,targetname,targetclassname):
    """
    Searches for a function by matching its byte signature at function entry points.
    """
    # Convert the signature from string format to a list of bytes
    byte_list = []
    for i in range(0, len(signature), 2):
        byte_str = signature[i:i+2]
        if byte_str == '??':
            byte_list.append(-1)  # Use -1 to represent wildcard
        else:
            try:
                byte_list.append(int(byte_str, 16))
            except ValueError:
                print("Invalid byte string:", byte_str)
                return None

    # Get the current program, function manager, and memory
    program = currentProgram
    function_manager = program.getFunctionManager()
    memory = program.getMemory()

    #print("Starting signature search for:", signature)
    #print("Parsed byte list (with wildcards):", byte_list)

    potfunc = None
    minfails = 9999999
    potfuncname = "funkname"
    for function in function_manager.getFunctionsNoStubs(True):
        entry_point = function.getEntryPoint()

        try:
            # Read only the number of bytes needed for the signature
            func_bytes =  zeros(len(byte_list), 'b')  # Create a buffer for the bytes
            bytes_read = memory.getBytes(entry_point, func_bytes)
            func_bytes = twos_complement_array_to_decimal(func_bytes) #fuck you ghidra, fuck you!
            	
            if bytes_read != len(byte_list):
                print("Error reading memory: not enough bytes at", entry_point)
                continue
            
            #print("Function: ", function.getName() ," at address:", entry_point, "Function bytes:", list(func_bytes))
            # Compare the function's entry bytes with the signature
            currfails = matches_signature_num_fails(func_bytes, byte_list)
            currfails = currfails + abs(currfuncparams - function.getParameterCount())
            if currfails < minfails:
                #print("Signature matched at function starting at:", entry_point)
                minfails = currfails
                potfunc = entry_point
                potfuncname = function.getName()
                # Return the function's entry point address
			
        except MemoryAccessException as e:
            print("Error reading memory at address:", entry_point, "-", str(e))
            continue  # Skip if there's an error reading memory
	
    print("Couldnt find:",targetclassname+"::"+targetname,"==closest==>", potfuncname , "("+ str(minfails), "mismatches)")
    return potfunc  # Return None if no function is found

def matches_signature(func_bytes, signature_bytes):
    """
    Compares the function's bytes to the signature with wildcards (byte_list containing -1).
    """
    #print("Matching function bytes:", list(func_bytes), "with signature:", signature_bytes)
    
    # Compare each byte of the function to the signature
    for i in range(len(signature_bytes)):
        if signature_bytes[i] != -1 and func_bytes[i] != signature_bytes[i]:
            #print("Byte mismatch at position "+str(i)+": function byte "+str(func_bytes[i])+", signature byte "+str(signature_bytes[i]))
            return False  # Mismatch found (wildcards are ignored)
    
    return True  # All bytes matched
	
	
def matches_signature_num_fails(func_bytes, signature_bytes):
    """
    Compares the function's bytes to the signature with wildcards (byte_list containing -1).
    """
    #print("Matching function bytes:", list(func_bytes), "with signature:", signature_bytes)
    failcount = 0
    # Compare each byte of the function to the signature
    for i in range(len(signature_bytes)):
        if signature_bytes[i] != -1 and func_bytes[i] != signature_bytes[i]:
            #print("Byte mismatch at position "+str(i)+": function byte "+str(func_bytes[i])+", signature byte "+str(signature_bytes[i]))
            failcount = failcount +1
    
    return failcount  # All bytes matched


	
	
	

def python_to_java_byte_array(python_bytes):
    """
    Convert a Python bytearray (unsigned) to a Java byte[] (signed).
    Values above 127 are converted to their signed counterparts.
    """
    java_bytes = zeros(len(python_bytes), 'b')  # Create a Java byte array of the same length
    for i in range(len(python_bytes)):
        value = python_bytes[i]
        if value > 127:
            value -= 256  # Convert to signed byte (Java)
        java_bytes[i] = value
    return java_bytes




def find_or_create_namespace(class_name):
    """
    Finds or creates the namespace (class) with the given name.
    """
    symbol_table = currentProgram.getSymbolTable()

    # Check if the namespace already exists
    global_namespace = currentProgram.getGlobalNamespace()
    namespace = symbol_table.getNamespace(class_name, global_namespace)
    
    if namespace is None:
        try:
            # Create the new namespace (class)
            namespace = symbol_table.createNameSpace(global_namespace, class_name, SourceType.USER_DEFINED)
            #print("Namespace " + class_name + " created.")
        except InvalidInputException as e:
            print("Error creating namespace " +class_name + "e: " + str(e))
            return None
    #else:
        #print("Namespace " +class_name+ " already exists.")

    return namespace

def find_function_by_signature(byte_signature):
    """
    Finds the function by searching for the given byte signature in the program's memory.
    """
    memory = currentProgram.getMemory()
    address = currentProgram.getMinAddress()
    end_address = currentProgram.getMaxAddress()
    
    # Convert the byte signature from string to a byte array
    byte_array = bytearray.fromhex(byte_signature)
    
    while address.compareTo(end_address) < 0:
        try:
            # Search for the byte sequence in memory
            found_address = memory.findBytes(address, python_to_java_byte_array(byte_array), None, True, TaskMonitor.DUMMY)
            if found_address is not None:
                #print("Found function signature at "+str(found_address))
                return found_address
        except MemoryAccessException:
            # Handle inaccessible memory regions
            pass
        # Increment address to continue search
        address = address.add(1)
    
    print("No matching function found for signature {byte_signature}")
    return None

def create_function_in_namespace(class_name, function_name, address):
    """
    Creates a function at the specified address in the specified class/namespace.
    """
    global countfuncfails
    global countfuncsucksex
    if not isinstance(address, Address):
		find_closest_function_address(address,function_name,class_name)
		countfuncfails = countfuncfails +1
		return None
		#function_name = function_name + "???"
		#if not isinstance(address, Address):
		#	print("Address not found for: ",class_name ,":", function_name)
		#	return None
		#print("This is the right function? : ",class_name ,":", function_name)
	
    namespace = find_or_create_namespace(class_name)
    if namespace is None:
        print("Failed to create or find the namespace.")
        return None

    # Check if the function already exists at the given address
    function = getFunctionAt(address)
    #print("Function " +function_name+ " already exists ")
    # Optionally, associate it with the namespace if it's not already
    function.setParentNamespace(namespace)
    function.setName(function_name,SourceType.IMPORTED)
    #print("Function " +function_name+ " is now associated with namespace " +class_name)
    countfuncsucksex = countfuncsucksex +1
    return function


def parse_zhl_file(file_path):
    functions = []

    with open(file_path, 'r') as file:
        content = file.read().replace("unsigned ", "").replace("static ", "").replace("cleanup ", "").replace(" *", "*") #I dont care about this for now, and it makes the regex trickier, lol
        
        matches = patterns['function_signature'].findall(content)
        
        for match in matches:
            address, callreg, returntype, classname, functionname, params = match
            
            if not callreg:
                callreg = ""
			
            function_data = {
                'address': address,
                'callreg': callreg,
                'returntype': returntype,
                'classname': classname,
                'functionname': functionname,
                'params': params
            }
            functions.append(function_data)
            currfuncparams = params.count(',')
            if callreg == "__thiscall":
                currfuncparams = currfuncparams +1
            create_function_in_namespace(classname, functionname, find_function_address(address))
            if count_function_address(address) > 1:
                print("Function " + classname + ":" + functionname + " matches multiple times.")
			
    return functions

def parse_all_zhl_files():
    all_functions = []
    
    # Traverse the current directory and find all .zhl files
    for filename in os.listdir(zhlfunctionspath):
        if filename.endswith('.zhl'):
            functions = parse_zhl_file(zhlfunctionspath+"\\" + filename)
            all_functions.extend(functions)
    
    # Output the result to a JSON file
    #with open('functions.json', 'w') as json_file:
     #   json.dump(all_functions, json_file, indent=4)

#zhl func registering shenanigans


if __name__ == "__main__":
	fm = currentProgram.getFunctionManager()
	fn = fm.getFunctionContaining(currentAddress)

	start_at = askChoice("makesig", "Action:", MAKE_SIG_AT.values(), MAKE_SIG_AT['fn'])
	
	# we currently don't expose min_length
	# TODO: rework askChoice to use a custom panel with all options
	
	if start_at == MAKE_SIG_AT['zhlcheckup']:
		parse_all_zhl_files()
		global countfuncfails
		global countfuncmult
		global countfuncsucksex
		print("Broken Function Sigs:", countfuncfails)
		print("Duplicate Function Matches:", countfuncmult)
		print("Functions Identified:", countfuncsucksex)
	else:
		if not fn:
			printerr("Not in a function")
		process(start_at, min_length = 1)