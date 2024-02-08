-- LUA COMMENT

--DEFINE THE BASIC SCENE PROPERTIES

-- BACKGROUND

background = {}
background["Color"] = {0,  0, 1, 1}
background["Texture"] = {}

WIDTH = 1920
HEIGHT = 1080

--SCENE CAMERA

--scene_camera = {"m_eye", "m_center", "m_up"}

scene_camera_m_eye = {x=0, y=2, z=5}
scene_camera_m_center = {x=0, y=0, z=0}
scene_camera_m_up = {x=0 , y=1, z=0}

--LIGHTING METHODS

light_position = {x=1, y=30, z=5}
light_color = {x=0, y=0, z=0}
sky_color = {x =0.8, y= 0.8, z = 0.5}
ground_color = {x = 0.1, y= 0.5, z=0.9}


shader = {}
shader["VShaderPath"] = "C:/dev/Silindokuhle15/Spring/Assets/Shaders/RndVertexShader.glsl"
shader["FShaderPath"] = "C:/dev/Silindokuhle15/Spring/Assets/Shaders/RndFragmentShader.glsl"

--material = {Shader= "", Texture = "", mtl_path = ""}
--material_Shader= Shader
--material_texture = "C:/dev/Silindokuhle15/Spring/Assets/Textures/crate_1.png"
material_texture = "C:/dev/Silindokuhle15/Spring/Assets/Objects/kenney_mini-skate/Models/OBJ format/Textures/colormap.png"

mtl_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/kenney_mini-skate/Models/OBJ format/steps.mtl"


--MESH COMPONENT, TRANSFORMS

--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/Castle/Castle OBJ.obj"
--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/House/Room.obj"
obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/House/Room.obj"
--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/kenney_mini-skate/Models/OBJ format/steps.obj"
--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/Cube/Cube.obj"
--obj_path = "C:/dev/Silindokuhle15/Spring/Assets/Objects/Tree/Lowpoly_tree_sample.obj"
--obj_path = "Grid"


--[====[
for k, v in pairs(_G) do
	--print(k,v)
	if type(v) == type({}) then
		print(k, v, #v)
	end
end
--]====]

