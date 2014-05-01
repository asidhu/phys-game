

function start()
	ResourceManager:registerTexture(1,"./reddiamond.png")
	ResourceManager:registerTexture(2,"./orangediamond.png")
	ResourceManager:registerTexture(3,"./bluediamond.png")
	ResourceManager:registerTexture(4,"./CloudySunset.png")
	Graphics:setupViewport(100,-100,0)
	Scene:setSceneBounds(-100,100,100,-100)
	Scene:setCameraBounds(-1,1,1,-1)
	local lay = Scene:getLayer(1)
	lay:constructButton(50,30,10,10,2,3,1)
	local gui = Scene:getLayer(0)
	gui:constructTexture(400,100,800,200,4)
	gui:setScrolling(0)	
	gui:setupDimensions(4,4)
	
	
	ActorManager:constructWall(0,-30,400,4,0,1)
	ActorManager:constructWall(0,50,50,8,0,1)
	
	local world = Scene:getLayer(2)
	
	world:constructWorld(Scene)
	
	
	player = ActorManager:getActorByID(1,5,5)
	player:setXY(-50,10):setMass(1):setTexture(1)
end


function update(timestep)
	local l,r,t,b = Scene:getCameraBounds()
	local bl,br,bt,bb = Scene:getSceneBounds()
	local pX,pY = player:getXY()
	local w = r-l
	local h = t-b
	l=pX-w/2
	r=pX+w/2
	t=pY+h*.7
	b=pY-h*.3
	if l < bl then
		r = r+ bl-l
		l=bl
	end
	if r > br then
		l=l+br-r
		r=br
	end
	if t > bt then
		b = b+ bt-t
		t=bt
	end
	if b < bb then
		t = t+ bb-b
		b=bb
	end
	Scene:setCameraBounds(l,r,t,b)
end


function evtButton(id)
	

end