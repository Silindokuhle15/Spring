-- LUA COMMENT

--DEFINE THE BASIC SCENE PROPERTIES

-- BACKGROUND

background = {}
background["Color"] = {0,  0, 1, 1}
background["Texture"] = {}

--SCENE CAMERA

--scene_camera = {"m_eye", "m_center", "m_up"}

scene_camera_m_eye = {x=0, y=0, z=0}
scene_camera_m_center = {X=0, y=0, z=5}
scene_camera_m_up = {x=0 , y=1, z=0}

--LIGHTING METHODS

shader = {}
shader["VShaderPath"] = "C:/dev/Silindokuhle15/Spring/Assets/Shaders/RndVertexShader.glsl"
shader["FShaderPath"] = "C:/dev/Silindokuhle15/Spring/Assets/Shaders/RndFragmentShader.glsl"


texture = {}
texture["Path"] = "C:/dev/Silindokuhle15/Spring/Assets/Textures/crate_1.png"

material = {}
material["Shader"] = Shader
material["Texture"] = texture
material["mtl_path"] = "C:/dev/Silindokuhle15/Spring/Assets/Objects/Tank/mytank.mtl"


--MESH COMPONENT, TRANSFORMS

--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/Castle/Castle OBJ.obj"
obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/85-cottage_obj/cottage_obj.obj"

--[====[
for k, v in pairs(_G) do
	--print(k,v)
	if type(v) == type({}) then
		print(k, v, #v)
	end
end
--]====]

