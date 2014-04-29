

function start()
	ResourceManager:registerTexture(1,"./reddiamond.png")
	ResourceManager:registerTexture(2,"./orangediamond.png")
	ResourceManager:registerTexture(3,"./bluediamond.png")
	ResourceManager:registerTexture(4,"./CloudySunset.png")
	Graphics:setupViewport(100,-100,0)
	Scene:setSceneBounds(-1,1,1,-1)
	Scene:setCameraBounds(-1,1,1,-1)
	local lay = Scene:getLayer(1)
	lay:constructButton(50,30,10,10,2,3,1)
	local gui = Scene:getLayer(0)
	gui:constructTexture(400,100,800,200,4)
	gui:setScrolling(1)	
	gui:setupDimensions(4,4)
	
	
	ActorManager:constructWall(0,-30,400,4,0,1)
	ActorManager:constructWall(0,50,50,8,0,1)
	
	local world = Scene:getLayer(2)
	
	world:constructWorld(Scene)
	
	
	local act = ActorManager:getActorByID(1,5,5)
	act:setXY(-50,10):setMass(1)
	print("lol")
end


function update(timestep)
	local l,r,t,b = Scene:getCameraBounds()
end


function evtButton(id)
	

end