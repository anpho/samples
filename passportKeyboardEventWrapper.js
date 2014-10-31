/*
 * Sample Binding Code, NOTE that this event should be binded after "deviceready" was fired.
 * document.addEventListener("deviceready", function () {
 *   document.addEventListener("touchenabledkeyboard", touchKeyboardCallback);
 *  });
 *
 */

var touchHandler=function(event){
	//event is swipedown  / swipeup  / swipeleft  / swiperight 
	//document.getElementById('app').innerHTML = event
  //handle events here.
}

var touchenabledkeyboardEvents={};

function touchKeyboardCallback(event) {
    event.touches.forEach(
        function (touch){
            console.log(JSON.stringify(touch);
            if (touch.state=="TouchReleased"){
	            var originalPoint=touchenabledkeyboardEvents[touch.identifier];
	            if (originalPoint){
		            var dx=touch.keyboardX - originalPoint.x;
		            var dy=touch.keyboardY - originalPoint.y;
		            if (Math.abs(dx)>Math.abs(dy)){
			            if (dx>0){
				            touchHandler("swiperight")
			            }else{
				            touchHandler("swipeleft")
			            }
		            }else{
			            if (dy>0){
				            touchHandler("swipedown")
			            }else{
				            touchHandler("swipeup")
			            }
		            }
		            originalPoint=touchenabledkeyboardEvents[touch.identifier]=null;
	            }
            }else if (touch.state=="TouchPressed"){
	            touchenabledkeyboardEvents[touch.identifier] = {
		            x:touch.keyboardX,
		            y:touch.keyboardY,
		            t:event.timeStamp
	            }
            }
        }
    )
}
