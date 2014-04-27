

function start()
	ResourceManager:registerTexture(1,"./reddiamond.png")
	ResourceManager:registerTexture(2,"./orangediamond.png")
	ResourceManager:registerTexture(3,"./bluediamond.png")
	ResourceManager:registerTexture(4,"./CloudySunset.png")
	Graphics:setupViewport(40,-40,0)
	local lay = Scene:getLayer(1)
	lay:constructButton(50,30,10,10,2,3,1)
	local gui = Scene:getLayer(0)
	Scene:setSceneBounds(-1,1,1,-1)
	Scene:setCameraBounds(-1,1,1,-1)
	gui:constructTexture(40,10,80,20,4)
	gui:setScrolling(1)	
	gui:setupDimensions(4,4)
	
	
	ActorManager:constructWall(0,-30,400,4,0,1)
	
	local world = Scene:getLayer(2)
	
	world:constructWorld(Scene)
	
	
	local act = ActorManager:getActorByID(1,10,10)
	act:setXY(-50,10):setMass(1)
	print("lol")
end


function update(timestep)
	local l,r,t,b = Scene:getCameraBounds()
end


function evtButton(id)
	

end