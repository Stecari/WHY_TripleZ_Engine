import os
import subprocess
import CheckPython

# Make sure everything we need is installed
CheckPython.ValidatePackages()

import Vulkan

# Change from Scripts directory to root
os.chdir('../')

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed.")

# if (not Vulkan.CheckVulkanSDKDebugLibs()):
#     print("Vulkan SDK debug libs not found.")

print("Running premake...")
#subprocess.call(["vendor/premake/bin/premake5.exe", "vs2019"])
subprocess.call(["vender/premake/premake5.exe", "vs2022"])
            #传递一个要执行的程序/命令等，后面的是传经去的参数，可以参考GenerateProject里面写的看一下
