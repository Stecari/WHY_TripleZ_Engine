import subprocess
import pkg_resources

def install(package):   #格式化字符串，字符串前面有f说明这个字符串中间用{}里面时个变量
	print(f"Instaling {package} module")
	subprocess.check_call(['python', '-m', 'pip', 'install'], package)

def ValidatePackage(package):
	required  = { package }
	installed = { pkg.key for pkg in pkg_resources.working_set }
	missing = required - installed

	if missing:
		install(package)

def ValidatePackages():
	ValidatePackage("requests")
	ValidatePackage("fake-useragent")