function onCreatePost()
	for i,characters in pairs({'boppers', 'grunt', 'radio', 'springtrap'}) do
		precacheImage('attraction/'..characters..'S')
	end
	precacheImage('characters/kookS')
	precacheImage('characters/kookSG')
	precacheImage('characters/pookS')
	
	initLuaShader('HARDLIGHTS')
	initLuaShader('rim_light')
	
	makeLuaSprite('rim', nil, 0, 0)
	addLuaSprite('rim', true)
	makeLuaSprite('rimExtras', nil, 0, 0)
	makeLuaSprite('rimThick', nil, 20)
	--setProperty('rim.x', 126259)
	--setProperty('rim.y', 3392)
	setSpriteShader('rim', 'rim_light')
	setShaderFloatArray('rim', 'u_color', {0.1, 0.1, 0.1})
	setShaderFloat('rim', 'u_brightness_ambient', 0)
	setShaderFloat('rim', 'u_brightness_glow', 0)
	setShaderFloat('rim', 'u_samples', getProperty('rimThick.x'))
	setShaderFloat('rim', 'u_thickness', getProperty('rimThick.x'))
	for i,parts in pairs({'boppers', 'springtrap', 'grunt', 'desk', 'room', 'outside', 'chair', 'board', 'bushes'}) do
        runHaxeCode([[ game.getLuaObject(']]..(parts)..[[').shader = game.getLuaObject("rim").shader; ]])
    end
	runHaxeCode([[
		if (game.gf != null) game.gf.shader = game.getLuaObject("rim").shader;
		game.dad.shader = game.getLuaObject("rim").shader;
		game.boyfriend.shader = game.getLuaObject("rim").shader;
				]])
end

function onCountdownStarted()
	if isStoryMode then 
		setProperty('cameraSpeed', 0)
		setProperty('camGame.zoom', 0.7) 
	end
end

function onEvent(name)
	if name == 'Change Character' then
		runHaxeCode([[
		if (game.gf != null) game.gf.shader = game.getLuaObject("rim").shader;
		game.dad.shader = game.getLuaObject("rim").shader;
		game.boyfriend.shader = game.getLuaObject("rim").shader;
				]])
	end
end

function onSongStart()
	setProperty('cameraSpeed', 1)
	setProperty('camZooming', true)
	setProperty('camZoomingMult', 1.2)
end

function onBeatHit()
	if beatHitFuncs[curBeat] then 
		beatHitFuncs[curBeat]() -- Executes function at curStep in stepHitFuncs
	end
end

beatHitFuncs = {
	
	[30] = function()
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: 126259, y: 3392}, (Conductor.crochet / 1000) * 1.4));
					]])
		doTweenX('rimExtrasXTween', 'rimExtras', 1.2, (crochet / 1000) * 1.4)
	end,
	
	[32] = function()
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: -game.getLuaObject('rim').x * 0.5, y: -game.getLuaObject('rim').y}, (Conductor.crochet / 1000) * 1.4));
					]])
		doTweenX('rimExtrasXTween', 'rimExtras', 0.7, (crochet / 1000) * 1.4)
		setShaderFloatArray('rim', 'u_color', {0, 0.2, 0})
	end,
	
	[46] = function()
		doTweenX('rimExtrasXTween', 'rimExtras', 1.1, (crochet / 1000) * 1.4)
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: 126259, y: 3392}, (Conductor.crochet / 1000) * 1.4));
					]])
	end,
	
	[48] = function()
		doTweenX('rimExtrasXTween', 'rimExtras', 0.4, (crochet / 1000) * 1.4)
		setShaderFloatArray('rim', 'u_color', {0.4, 0.4, 0})
	end,
	
	[60] = function()
		doTweenX('rimExtrasXTween', 'rimExtras', 0, (crochet / 1000) * 5)
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: -game.getLuaObject('rim').x * 0.5, y: -game.getLuaObject('rim').y}, (Conductor.crochet / 1000) * 5));
					]])
	end,
	
	[116] = function()
		doTweenX('camGameTargetXTween', 'camGame.target', getProperty('camGame.target.x') * 2, 'quadInOut')
		doTweenX('rimExtrasXTween', 'rimExtras', 0.2, (crochet / 1000) * 2)
		doTweenX('rimXTween', 'rim', 126259, (crochet / 1000) * 2)
		doTweenX('rimYTween', 'rim', 3392, (crochet / 1000) * 2)
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: 126259, y: 3392}, (Conductor.crochet / 1000) * 2));
					]])
		setShaderFloatArray('rim', 'u_color', {0.1, 0.1, 0.1})
	end,
	
	[118] = function()
		runHaxeCode([[
			game.moveCamera(true);
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: -game.getLuaObject('rim').x * 0.5, y: -game.getLuaObject('rim').y}, (Conductor.crochet / 1000) * 2));
					]])
		doTweenX('rimExtrasXTween', 'rimExtras', 0, (crochet / 1000) * 2)
	end,
	
	[188] = function()
		doTweenX('rimExtrasXTween', 'rimExtras', 1.1, (crochet / 1000) * 4)
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: 126259, y: 3392}, (Conductor.crochet / 1000) * 4));
					]])
	end,
	
	[192] = function()
		doTweenX('rimExtrasXTween', 'rimExtras', 0, (crochet / 1000) * 2)
	end,
	
	[224] = function()
		runHaxeCode([[
			game.modchartTweens.set('rimTween', FlxTween.tween(game.getLuaObject('rim'), {x: -game.getLuaObject('rim').x * 0.5, y: -game.getLuaObject('rim').y}, (Conductor.crochet / 1000) * (4 * 1.4)));
					]])
		doTweenX('rimExtrasXTween', 'rimExtras', 1.1, (crochet / 1000) * (4 * 1.4))
	end,
}

function onUpdatePost(elapsed)
	setShaderFloatArray('rim', 'u_dir', {getProperty('rim.x'), getProperty('rim.y')})
	setShaderFloat('rim', 'u_mix', getProperty('rimExtras.x'))
end