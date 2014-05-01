


t_text1=5
t_text2=6
t_text3=7

function start()
	ResourceManager:registerTexture(1,"./reddiamond.png")
	ResourceManager:registerTexture(2,"./orangediamond.png")
	ResourceManager:registerTexture(3,"./bluediamond.png")
	ResourceManager:registerTexture(4,"./CloudySunset.png")
	ResourceManager:registerTexture(5,"./TutorialText1.png")
	ResourceManager:registerTexture(6,"./TutorialText2.png")
	ResourceManager:registerTexture(7,"./TutorialText3.png")
	
	Graphics:setupViewport(100,-100,0)
	Scene:setSceneBounds(-100,800,100,20)
	Scene:setCameraBounds(-1,1,1,-1)
	local lay = Scene:getLayer(1)
	lay:setScrolling(1)
	lay:setupDimensions(2,2)
	lay:constructTexture(100,00,100,60,5)
	--lay:constructButton(50,30,10,10,2,3,1)
	local gui = Scene:getLayer(0)
	gui:constructTexture(400,100,800,200,4)
	gui:setScrolling(0)	
	gui:setupDimensions(4,4)
	
	
	
	
	-- floor
	ActorManager:constructWall(200,-60,800,62,0,1)
	--left wall
	ActorManager:constructWall(-200,30,4,120,0,1)
	--roof
	ActorManager:constructWall(200,90,800,4,0,1)
	--platform
	ActorManager:constructWall(0,50,50,8,0,1)
	--wall above trap1
	ActorManager:constructWall(150,60,100,60,0,1)
	
	--wall with button
	ActorManager:constructWall(230,35,60,4,0,1)
	--wall with button
	ActorManager:constructWall(260,55,40,4,0,1)
	--wall with button again
	ActorManager:constructWall(280,60,5,60,0,1)
	--button activated wall
	button1wall=ActorManager:constructWall(280,0,5,60,0,1)
	
	
	
	
	local world = Scene:getLayer(2)
	
	world:constructWorld(Scene)
	
	
	player = ActorManager:getActorByID(1,5,5)
	player:setXY(230,20):setMass(1):setTexture(1)
	
	--local enemy = ActorManager:getActorByType(1,3,3)
	--enemy:setTexture(3):setMass(1)
	
	
	--first obstacle
	
	trap1 = ActorManager:constructWall(100,50,6,60,0,1):lockRotation():setV(0,-100)
	trap2 = ActorManager:constructWall(105,-80,6,50,0,1):lockRotation():setV(0,100)
	trap1Counter=0
	
	trap3 = ActorManager:constructWall(150,30,6,50,0,1):lockRotation():setV(0,-60)
	trap4 = ActorManager:constructWall(165,-80,6,50,0,1):lockRotation():setV(0,60)
	trap5 = ActorManager:constructWall(160,30,6,50,0,1):lockRotation():setV(0,-60)
	trap6 = ActorManager:constructWall(155,-80,6,50,0,1):lockRotation():setV(0,100)
	trap2Counter=0
	trap3Counter=0
	
	button1 = ActorManager:getActorByType(2,2,8):setXY(205,80):setTexture(2):setButtonTime(5)
	
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
	if trap1Counter > .5 then
		local vx,vy=trap1:getV()
		trap1:setV(vx,-vy)
		local vx,vy=trap2:getV()
		trap2:setV(vx,-vy)
		trap1Counter=0
	end
	trap1Counter=trap1Counter+timestep
	if trap2Counter > .8 then
		local vx,vy=trap3:getV()
		trap3:setV(vx,-vy)
		local vx,vy=trap4:getV()
		trap4:setV(vx,-vy)
		local vx,vy=trap5:getV()
		trap5:setV(vx,-vy)
		trap2Counter=0
	end
	trap2Counter=trap2Counter+timestep
	if trap3Counter > .6 then
		local vx,vy=trap6:getV()
		trap6:setV(vx,-vy)
		trap3Counter=0
	end
	trap3Counter=trap3Counter+timestep
	
	if button1:isToggled() then
		button1:setTexture(1)
		local r = button1wall:getRot()
		if r<1.5 then
			button1wall:setRot(r+.06)
		end
	else 
		button1:setTexture(2)
		local r = button1wall:getRot()
		if r>0 then
			button1wall:setRot(r-.06)
		end
	end
	
end


function evtButton(id)
	

end