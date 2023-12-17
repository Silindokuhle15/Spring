-- LUA COMMENT

function find_neighbor( vertex, dir, map_size )
	-- body
	local dx = dir[1]
	local dy = dir[2]
	local neighbour = {}

	if (vertex[1] + dx <= map_size and  vertex[1] + dx >= 1) and (vertex[2] + dy <= map_size and vertex[2] + dy >= 1 ) 
	then
		neighbour =  {vertex[1] + dx, vertex[2] + dy}
	end
	return neighbour
end


function find_neighbors(map_size, vertex)
	local dummy_neighbours = {}
	local dir = {
		{  1,  0},
		{  0,  1},
		{ -1,  0},
		{  0, -1}
	}
	for index, dir_at_index in pairs(dir) do
		local dx = dir_at_index[1]
		local dy = dir_at_index[2]

		local new_neighbour = find_neighbor(vertex, {dx, dy}, map_size)
		if  new_neighbour == nil 
		then
		else
			local x = new_neighbour[1]
			local y = new_neighbour[2]
			if not( x == nil or   y == nil ) then
				--print("Inserting neighbour" , "{ ", x, "," ,y, " }")
				table.insert(dummy_neighbours, {x, y})
				--dummy_neighbours[{x, y}] = {x, y}

			end
		end
	end
	return dummy_neighbours
end

function find_new_neighbours( all_neighbours, visited )
	-- body
	local new_neighbours = {}
--[====[
	for index=1, #all_neighbours do

		nth_neighbour = all_neighbours[index]
		print("nth_neighbour", nth_neighbour[1], nth_neighbour[2])
		if not (visited[nth_neighbour] == nth_neighbour[1] and visited[nth_neighbour] == nth_neighbour[2]) then
			table.insert(new_neighbours, nth_neighbour)
		end		

	end
--]====]
	for index, value in ipairs(all_neighbours) do
		x = value[1]
		y = value[2]
		if not (visited[{x,y}] == {x,y}) then
			table.insert(new_neighbours, value)
			--print("("..x..","..y..") has not been visited before")
		end

	end
	return new_neighbours
end

function DFS( map_size, start_pos, end_pos, visited, output_path)
	-- body
	table.insert(output_path, start_pos)
	--print("Calling DFS with the arguements :"..map_size.." ("..start_pos[1]..","..start_pos[2]..")")

	if (start_pos[1] == end_pos[1] and start_pos[2] == end_pos[2]) then
		print("Reached the end")
		return
	else
		if not visited[start_pos] then
			table.insert(visited, start_pos)
			local all_neighbours = find_neighbors(map_size, start_pos)
			local new_neighbours = find_new_neighbours(all_neighbours, visited)

		-- RANDOMLY SELECT ANY OF THE NEW NEIGHBOURS TO VISIT
		local rand = math.random(#new_neighbours)
		local next_pos = new_neighbours[rand]

		x = next_pos[1]
		y = next_pos[2]

		v_x = visited[next_pos]

		if not (visited[next_pos] == {x,y}) then
			return DFS(map_size, next_pos, end_pos, visited, output_path)
		else
			print("Going Back")
			return DFS(map_size, visited[-1], end_pos, visited, output_path)
		end
		end
	end
end

map_size = 5
initial_vertex = {1,1}
last_vertex = {map_size, map_size}

Visited = {initial_vertex}
Output_Path = {}
--Visited = {}

DFS(map_size, initial_vertex, last_vertex, Visited, Output_Path)

--print("lenght of queue with duplicates " .. #Visited.. "\n")

-- NOW PRINT THE queue
-- REMOVE PUBLICATES FROM QUEUE

function remove_duplicates(input)
	-- body
	local hash = {}
	local res = {}

	for _,z in ipairs(input) do
	   if (not hash[z]) then
		   res[#res+1] = z -- you could print here instead of saving to result table if you wanted
		   hash[z] = true
		else	
			print("duplicate found", z)
	   end
	end
	return res
end



print("lenght of path with duplicates " .. #Output_Path.. "\n")

for i=1, #Output_Path do
	print("("..Output_Path[i][1]..","..Output_Path[i][2]..")")
end