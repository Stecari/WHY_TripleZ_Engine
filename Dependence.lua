VULKAN_SDK = os.getenv("VULKAN_SDK")		--这个时获取环境变量，要配置电脑环境变量的，下载完发现自动配置的


IncludeDir = {}
IncludeDir["GLFW"] = "vender/GLFW/include"
IncludeDir["Glad"] = "vender/Glad/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["ImGui"] = "TribleZ/Gitvender/ImGui"
IncludeDir["glm"] = "TribleZ/Gitvender/glm"
IncludeDir["entt"] = "TribleZ/Gitvender/entt"
IncludeDir["stb_image"] = "TribleZ/Gitvender/stb_image"
IncludeDir["yaml_cpp"] = "TribleZ/Gitvender/yaml_cpp/include"
IncludeDir["Box2D"] = "TribleZ/Gitvender/Box2D/include"
IncludeDir["Box2D_3"] = "TribleZ/Gitvender/Box2D_3/include"
IncludeDir["ImGuizmo"] = "TribleZ/Gitvender/ImGuizmo"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Bin"] = "%{VULKAN_SDK}/Bin"
--LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/GameEngineLightWeight/vendor/

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
--Library["glslang"] = "%{LibraryDir.VulkanSDK}/glslang.lib"
--Library["shaderc"] = "%{LibraryDir.VulkanSDK}/shaderc.lib"
--Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"		--这个老版本的，新的换成一个动态库了
--Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK_Bin}/VkLayer_khronos_validation.dll"	--考虑删掉，搜出来的

Library["ShaderC"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools_Release"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"

Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"