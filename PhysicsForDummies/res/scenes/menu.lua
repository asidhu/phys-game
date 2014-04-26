

function start()
	ResourceManager:registerTexture(1,"./reddiamond.png")
	ResourceManager:registerTexture(2,"./orangediamond.png")
	ResourceManager:registerTexture(3,"./bluediamond.png")
	local lay = Scene:getLayer(0)
	lay:constructTexture(30,30,50,50,1)
	local gui = Scene:getLayer(1)
	Scene:setSceneBounds(-1,1,1,-1)
	Scene:setCameraBounds(-1,1,1,-1)
	gui:constructWorld(Scene)
	gui:constructButton(50,30,10,10,2,3,1)
	gui:setScrolling(1)	
	gui:setupDimensions(4,4)
	print("lol")
end


function update(timestep)
	local l,r,t,b = Scene:getCameraBounds()
	Scene:setCameraBounds(l+.1,r,t,b)
end