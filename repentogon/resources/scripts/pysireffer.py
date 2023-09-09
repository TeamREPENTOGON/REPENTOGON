#!/usr/bin/python3
print("""
  _____        _____ _           __  __          
 |  __ \      / ____(_)         / _|/ _|         
 | |__) |   _| (___  _ _ __ ___| |_| |_ ___ _ __ 
 |  ___/ | | |\___ \| | '__/ _ \  _|  _/ _ \ '__|
 | |   | |_| |____) | | | |  __/ | | ||  __/ |   
 |_|    \__, |_____/|_|_|  \___|_| |_| \___|_|   
         __/ |                                   
        |___/                                   """)
print("PySireffer - the REPENTOGON downloader/updater")

import os
import sys
import time
import json
import zipfile
import base64
import urllib.request

if not os.path.isfile("isaac-ng.exe"):
	os.chdir("../..")

if not os.path.isfile("isaac-ng.exe"):
	print("First put this script in the same directory as the game itself! Exiting...")
	raise SystemExit

token="" # go to https://github.com/settings/tokens/ to generate a token with "repo" access!
if len(sys.argv)>1:
	token=sys.argv[1]	# accepts token as an argument without writing to any file
	if token[:3]!="ghp":
		print("Warning: Invalid token provided as argument!")
		token=""
if len(token)==0:
	if os.path.isfile(".pysireffer_token"):
		encoded=""
		with open('.pysireffer_token', 'r') as file:
			for line in file:
				if line[:3]=="ghp":
					token=line
					print("Warning: plain text token! Converting to b64!")
					encoded=base64.b64encode(bytes(token,"utf-8")).decode("utf-8")
					break
				else:
					line=line.replace(" ","")+"=="
					try:
						token=base64.b64decode(bytes(line,"utf-8")).decode("utf-8")
					except Exception:
						continue
					break
		if token[:3]!="ghp":
			print("Warning: Invalid token in a text file!")
			token=""
		if len(encoded)>0:
			with open('.pysireffer_token', 'w') as file:
                        	encoded=encoded[:6]+"   "+encoded[6:-2]
                        	file.write(encoded)
	if len(token)==0:
		print("""
To use this script you'll need to provide a GitHub Personal Access Token (PAT) with \"repo\" scope.
Token will be saved in a \".pysireffer_token\" file in the same location as \"isaac-ng.exe\" for later use.

If you do not have a token yet, please generate it at
https://github.com/settings/tokens/
using the account that has access to the repo!

If you do not want your token to be saved as encoded text, you may provide it as an argument when launching the script instead.
""")
		while token[:3]!="ghp":
			token=str(input("Enter the token: "))
		with open('.pysireffer_token', 'w') as file:
			writedata=base64.b64encode(bytes(token,"utf-8")).decode("utf-8")
			writedata=writedata[:6]+"   "+writedata[6:-2]
			file.write(writedata)
token=token.replace("\n","")
orgname="IsaacZHL"
reponame="REPENTOGON"

url=f"https://api.github.com/repos/{orgname}/{reponame}/actions/artifacts?per_page=1"
data={
	"per_page": 1
}
headers={
	"Accept": "application/vnd.github+json",
	"Authorization": "token "+token,
	"X-GitHub-Api-Version": "2022-11-28"
}
req=urllib.request.Request(url,None,headers)
with urllib.request.urlopen(req) as resp:
	out=resp.read()
	out=json.loads(out)
sha=""
try:
	sha=out['artifacts'][0]['workflow_run']["head_sha"][:7]
except KeyError:
	print("Invalid token has been provided!")
	if os.path.isfile(".pysireffer_token"):
		os.rename('.pysireffer_token','.pysireffer_token.backup.{0}'.format(int(time.time())))
		raise SystemExit

if os.path.isfile("repentogon_commit"):
	with open('repentogon_commit', 'r') as file:
		for line in file:
			if line==sha:
				print("REPENTOGON is already up to date! Exiting...")
				raise SystemExit

print("Downloading commit {0} from {1}...".format(sha,out['artifacts'][0]["created_at"]))
downloadurl=out['artifacts'][0]["archive_download_url"]
req=urllib.request.Request(downloadurl,None,headers)
with urllib.request.urlopen(req) as resp:
	open("repentogon.zip",'wb').write(resp.read())
print("Download finished! Extracting...")
zipfile.ZipFile('repentogon.zip', 'r').extractall()
print("Deleting the archive...")
os.remove("repentogon.zip")

with open('repentogon_commit', 'w') as file:
    file.write(sha)
print("The REPENTOGON has been pysireffed, enjoy!")
