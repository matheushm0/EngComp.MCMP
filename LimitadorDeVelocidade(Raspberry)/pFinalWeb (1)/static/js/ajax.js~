var imported = document.createElement('speed.js');
imported.src = 'speed.js';
document.head.appendChild(imported); 

function MaxFreq() {
	var velocidade = $("#velocidade").val();
	var frequencia = $("#myRange").val();

	$.ajax({
		url: "http://10.110.2.121:8080/setVmax",
		method:"POST",
		data:JSON.stringify({f:frequencia,v:velocidade}),
		dataType:"json",
	   contentType: 'application/json',
		success:function(j){
			alert("OI");	
		}
	});
}

function AtualizaVelocimetro(background1, velocimetro) {
	$.ajax({
		url:"http://10.110.2.121:8080/getVAtual",
		method:"GET",
		data:{},
		dataType:"JSON",
		success:function(j){
			var x = parseFloat(j.v);
			console.log(x.toFixed(0));
			setSpeed(x.toFixed(0));
		}
	});
	var display = document.getElementById(background1).style.display;
	document.getElementById(background1).style.display = 'none';
	document.getElementById(velocimetro).style.display = 'block';
	setTimeout(function()
   {
      AtualizaVelocimetro('background1', 'velocimetro');
   }, 1000);
}