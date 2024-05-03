#pragma once

//这个文件是专门给客户端用户（sandbox）包含用的
#include <iostream>
#include "TribleZ_Core/Application.h"
#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Log.h"
#include "TribleZ_Core/Layer.h"
#include "ImGui/ImGuiLayer.h"

#include "TribleZ_Core/TimeStep.h"

#include "TribleZ_Core/Input.h"
#include "TribleZ_Core/Mouse_and_Key_Code/MouseButtonCodeTable.h"
#include "TribleZ_Core/Mouse_and_Key_Code/KeyCodeTable.h"
#include "TribleZ_Core/OrthoGraphicCameraContraller.h"

#include "Platform/OpenGL/OpenGLShader.h"

/*--------------渲染器----------------------------------*/
#include "TribleZ_Core/Render/Renderer.h"
#include "TribleZ_Core/Render/Renderer2D.h"
#include "TribleZ_Core/Render/RendererCommand.h"
#include "TribleZ_Core/Render/Shader.h"
#include "TribleZ_Core/Render/FrameBuffer.h"
#include "TribleZ_Core/Render/Texture.h"
#include "TribleZ_Core/Render/SubTexture2D.h"
#include "TribleZ_Core/Render/Buffer.h"
#include "TribleZ_Core/Render/VertexArray.h"
/*--------------渲染器----------------------------------*/
/*---------------场景-----------------------------------*/
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/Component.h"
/*---------------场景-----------------------------------*/



/*--------------键码值----------------------------------*/

#include "TribleZ_Core/Mouse_and_Key_Code/KeyCodeTable.h"
#include "TribleZ_Core/Mouse_and_Key_Code/MouseButtonCodeTable.h"

/*--------------键码值----------------------------------*/