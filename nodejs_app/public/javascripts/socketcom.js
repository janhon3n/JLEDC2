var socket = io('http://localhost:3000');


function setColor(color){
		socket.emit("setcolor", {color:color});
}

$(document).ready(function(){
	var triangleElement = $("#colorTriangle");
	socket.on('recievecolor', function(data){
		triangleElement.trigger("recieveColor", [ data.color ]);
	});
});