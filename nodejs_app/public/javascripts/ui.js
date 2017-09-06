$(document).ready(function(){
	
	//tab controls
	$(".tabButton").click(function(){
		$(".tabPage").hide();
		var childNum = Number($(".tabButton").index($(this)));
		$(".tabPage").eq(childNum).show();
	});
	
	
	var triangleElement = $("#colorTriangle");
	
	var colorTriangle = new ColorTriangle("#FF0000", {
		size: triangleElement.width(),
		padding:20,
	});
	
	colorTriangle.addEventListener('drag', function(){
		var rgb = colorTriangle.getRGB();
		uiToColor(rgb);
		setColor(rgb);
	});
	
	triangleElement.on("recieveColor", function(e, rgb){
		colorTriangle.setRGB(rgb[0], rgb[1], rgb[2]);
		uiToColor(rgb);
	});
	
	//color in rgb[0-1, 0-1, 0-1]
	function uiToColor(rgb){
		var byteRgb = [Math.floor(rgb[0]*255), Math.floor(rgb[1]*255), Math.floor(rgb[2]*255)];
		$("#rgbInfo").html(byteRgb[0]+"R "+byteRgb[1]+"G "+byteRgb[2]+"B");
		$("body").css({
			"background-color": "rgb("+byteRgb[0]+","+byteRgb[1]+","+byteRgb[2]+")"
		});
	}
	
	//Color in rgb[0-1, 0-1, 0-1] format
	function setColorFromProgram(color){
		uiToColor(color);
		setColor(color);
		colorTriangle.setRGB(color[0], color[1], color[2]);
	}
	
	colorTriangle.inject(triangleElement[0]);
	
	function createOptionElement(el){
		switch(el.attr("action-type")){
			case "hold":		
				el.html('Hold<div class="inputs"><input name="delay" type="number" min="1" max="60" step="0.1" value="1"><input type="hidden" name="color" value="#FFFFFF"></div>');
				
				var input = $('<BUTTON class="jscolorButton"></BUTTON>');
				el.find('.inputs').append(input);
				var picker = new jscolor(input[0], {
					valueElement: el.find("input[name='color']")[0],
					styleElement: el[0],
					required: false,
					refine: true,
					hash: true
				});
				break;
			case "sweep":
				el.html('Sweep<div class="inputs"><input type="number" name="revolutions" min="-10" max="10" step="1" value="0"><input name="delay" type="number" min="1" max="60" step="0.1" value="1"><input type="hidden" name="color" value="#FFFFFF"></div>');
				
				var input = $('<BUTTON class="jscolorButton"></BUTTON>');
				el.find('.inputs').append(input);
				var picker = new jscolor(input[0], {
					valueElement: el.find("input[name='color']")[0],
					styleElement: el[0],
					required: false,
					refine: true,
					hash: true
				});
				break;
		}
	}
	
	//RUNNING THE PROGRAM
	var framesPerSecond = 60;
	var speed = 1;
	
	var timeout;
	function handleOption(el){
		switch(el.attr("action-type")){
			case "hold":
				setColorFromProgram(hexToRgb(el.find("input[name='color']").val()));
				timeout = setTimeout(moveToNext, el.find('input[name="delay"]').val() * 1000 / speed);
				break;
			case "sweep":
				var prev;
				if(el.is(":first-child")){
					prev = el.parent().find('.option:last-child');
				} else {
					prev = el.prev();
				}
				var startColor = hexToHSL(prev.find("input[name='color']").val());
				var endColor = hexToHSL(el.find("input[name='color']").val());
				var delay = el.find("input[name='delay']").val() * 1000;
				var steps = delay / 1000 * framesPerSecond;
				var hslSteps = []
				var hueDiff = (endColor['h'] - startColor['h']);
				if(hueDiff > 0.5) hueDiff -= 1;
				if(hueDiff < -0.5) hueDiff += 1;
				hueDiff += Number(el.find('input[name="revolutions"]').val());
				hslSteps['h'] = hueDiff / steps;
				hslSteps['s'] = (endColor['s'] - startColor['s']) / steps;
				hslSteps['l'] = (endColor['l'] - startColor['l']) / steps;
				console.log(hslSteps);
				sweepStep(1, steps, startColor, hslSteps)
				break;
		}
	}
	
	function sweepStep(step, steps, lastColor, hslSteps){
		setColorFromProgram(hslToRgb(lastColor['h'], lastColor['s'], lastColor['l']));
		
		if(step >= steps) {
				timeout = setTimeout(moveToNext, 1000 / framesPerSecond / speed);
		} else {
			lastColor['h'] += hslSteps['h'];
			if(lastColor['h'] > 1) lastColor['h'] -= 1;
			if(lastColor['h'] < 0) lastColor['h'] += 1;
			lastColor['s'] += hslSteps['s'];
			lastColor['l'] += hslSteps['l'];
			timeout = setTimeout(function(){
				sweepStep(step + 1, steps, lastColor, hslSteps);
			}, 1000 / framesPerSecond / speed);
		}
	}
	
	
	function moveToNext(){
		var current = $("#designBox .option[running='true']");
		var next;
		if(current.is(':last-child')){
			next = current.parent().find('.option:first');
		} else {
			next = current.next();
		}
		current.attr("running", "false");
		next.attr("running", "true");
		
		handleOption(next);
	}
	
	function stopProgram(){
		if(timeout) clearTimeout(timeout);
		$("#designBox .option").attr("running", "false");
	}
	
	function startProgramFromBeginning(){
		stopProgram();
		var firstOption = $("#designBox .option:first");
		firstOption.attr("running", "true");
		handleOption(firstOption);
	}

	
	//PROGRAM CONTROLS
	$("#design #designControl button[name='start']").click(function(){
		startProgramFromBeginning();
	});
	
	$("#design #designControl button[name='stop']").click(function(){
		stopProgram();
	});
	
	$("#design #designControl input[name='speed']").on('input', function(){
		$("#design #designControl span.speedInfo").html(speedToBPM($(this).val()));
		speed = $(this).val();
	});
	var tempoClickTime;
	$("#design #designControl button[name='tempo']").click(function(){
		currentTime = new Date().getTime();
		var tempoSpeed = 1/((currentTime - tempoClickTime)/1000);
		console.log(tempoSpeed);
		if(tempoSpeed < 3000 && tempoSpeed > 0.1){
			$("#design #designControl input[name='speed']").val(tempoSpeed);
			$("#design #designControl span.speedInfo").html(speedToBPM(tempoSpeed));
			speed = tempoSpeed;
		}
		tempoClickTime = currentTime;
	});
	
	function speedToBPM(speed){
		return Math.round(60 * 100 / (1/speed)) / 100;
	}
	
	
	
	//DRAGGING
	$("#designOptions div.option").draggable({
		connectToSortable: "#designBox",
		containment: $("#design"),
		helper: 'clone',
		revert: 'invalid',
		revertDuration: 50,
		start: function(event, ui){
			$("#design div#designOptions").attr("trashcanMode", "true");
		},
		stop: function(event, ui){
			$("#design div#designOptions").attr("trashcanMode", "false");
		}
	});
	$("#designOptions").droppable({
		tolerance: "fit",
		drop: function(event, ui){
			if(!(ui.draggable.hasClass("draggable"))){
				ui.draggable.animate({
					opacity:0
				}, 200, function(){ this.remove(); });
			}
		}
	});
	
	$("#designBox").sortable({
		revert: 50,
		containment: $("#design"),
		stop: function(event, ui){
			console.log();
			$("#design div#designOptions").attr("trashcanMode", "false");
			if($(ui.item).hasClass("draggable")){
				$(ui.item).html("");
				$(ui.item).removeClass("draggable").removeAttr("style");
				createOptionElement($(ui.item));
			}
		},
		start: function(event, ui){
			$("#design div#designOptions").attr("trashcanMode", "true");
		}
	});
	
	// COLOR SHIT
	function hexToRgb(hex) {
		var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		if(result){
			var res = [];
			res[0] = parseInt(result[1], 16) / 255;
			res[1] = parseInt(result[2], 16) / 255;
			res[2] = parseInt(result[3], 16) / 255;
			return res;
		}
		return null;
	}
	
	function hexToHSL(hex) {
	  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		r = parseInt(result[1], 16);
		g = parseInt(result[2], 16);
		b = parseInt(result[3], 16);
		r /= 255, g /= 255, b /= 255;
		var max = Math.max(r, g, b), min = Math.min(r, g, b);
		var h, s, l = (max + min) / 2;
		if(max == min){
		  h = s = 0; // achromatic
		}else{
		  var d = max - min;
		  s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
		  switch(max){
			case r: h = (g - b) / d + (g < b ? 6 : 0); break;
			case g: h = (b - r) / d + 2; break;
			case b: h = (r - g) / d + 4; break;
		  }
		  h /= 6;
		}
	  var HSL = new Object();
	  HSL['h']=h;
	  HSL['s']=s;
	  HSL['l']=l;
	  return HSL;
	}
	
	function hslToRgb(h, s, l){
		var r, g, b;

		if(s == 0){
			r = g = b = l; // achromatic
		}else{
			var hue2rgb = function hue2rgb(p, q, t){
				if(t < 0) t += 1;
				if(t > 1) t -= 1;
				if(t < 1/6) return p + (q - p) * 6 * t;
				if(t < 1/2) return q;
				if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
				return p;
			}

			var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
			var p = 2 * l - q;
			r = hue2rgb(p, q, h + 1/3);
			g = hue2rgb(p, q, h);
			b = hue2rgb(p, q, h - 1/3);
		}

		return [r, g, b];
	}
});
	