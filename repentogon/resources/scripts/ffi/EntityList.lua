ffi.cdef[[
	// stubs
	typedef struct {void *_;} EntityList;
	typedef struct {
		bool SubList;
		Entity ** Data;
		unsigned int Capacity;
		unsigned int Size;
	} EntityList_EL;
]]
