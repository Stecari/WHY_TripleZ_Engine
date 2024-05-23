import os
import subprocess
import sys
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

#获取环境变量
VULKAN_SDK = os.environ.get("VULKAN_SDK")
VULKAN_SDK_INSTALL_URL = "https://sdk.lunarg.com/sdk/download/1.3.280.0/windows/VulkanSDK-1.3.280.0-Installer.exe"
TRIPLEZ_VULKAN_VERTION = "1.3.280.0"
VULKAN_SDK_LOCAL_PATH = "vender/Vulkan/VulkanSDK.exe"

def installVulkanSDK():
    print("Downloading {} to {}".format(VULKAN_SDK_INSTALL_URL, VULKAN_SDK_LOCAL_PATH))
    Utils.DownloadFile(VULKAN_SDK_INSTALL_URL,  VULKAN_SDK_LOCAL_PATH)
    print('DONE!')
    print("Running VulkanSDK Installer!")
    os.startfile(os.path.abspath(VULKAN_SDK_LOCAL_PATH))
                        #返回绝对路径
    print("Re-run this script after installation")

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = Utils.YesOrNo()
    if(install):
        installVulkanSDK()
        quit()

def CheckVulkanSDK():
    if(VULKAN_SDK is None):
        print("You don't have the Vulkan SDK installed!")
        InstallVulkanPrompt()
        return False
    elif(TRIPLEZ_VULKAN_VERTION not in VULKAN_SDK):
        print(f"Located VulkanSDK at {VULKAN_SDK}")
        print(f"You don't have the correct Vulkan SDK version! (TrpleZ requires {TRIPLEZ_VULKAN_VERTION})")
        InstallVulkanPrompt()
        return False

    print(f"Correct VulkanSDK Located at {VULKAN_SDK}")
    return True



#新版本中不存在单独下载DEBUG库的链接了，都在那个链接里
# VulkanSDKDebugLibsURL = 'https://files.lunarg.com/SDK-1.2.170.0/VulkanSDK-1.2.170.0-DebugLibs.zip'
# OutputDirectory = "Hazel/vendor/VulkanSDK"
# TempZipFile = f"{OutputDirectory}/VulkanSDK.zip"

# def CheckVulkanSDKDebugLibs():
#     shadercdLib = Path(f"{OutputDirectory}/Lib/shaderc_sharedd.lib")
#     if (not shadercdLib.exists()):
#         print(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
#         print("Downloading", VulkanSDKDebugLibsURL)
#         with urlopen(VulkanSDKDebugLibsURL) as zipresp:
#             with ZipFile(BytesIO(zipresp.read())) as zfile:
#                 zfile.extractall(OutputDirectory)
#     print(f"Vulkan SDK debug libs located at {OutputDirectory}")
#     return True

