/*
 * Sample Binding Code, NOTE that this event should be binded after "deviceready" was fired.
 * 注意：此代码需在webworks中以以下方式绑定到事件，且只绑定一次。
 * document.addEventListener("deviceready", function () {
 *   document.addEventListener("touchenabledkeyboard", touchKeyboardCallback);
 *  });
 *
 */

var touchHandler=function(event){
  //event is swipedown  / swipeup  / swipeleft  / swiperight 
  //document.getElementById('app').innerHTML = event
  //handle events here.
  //在此函数内处理不同的事件。
}

// 事件存储区
// Event Storage
var touchenabledkeyboardEvents={};

// The least distance you move your finger on the keyboard to trigger this function.
// 触发事件的最小移动距离
var threshold=20;

function touchKeyboardCallback(event) {
    event.touches.forEach(
        function (touch){
            console.log(JSON.stringify(touch));
            if (touch.state=="TouchReleased"){
            	//取出这个事件的起点
	            var originalPoint=touchenabledkeyboardEvents[touch.identifier];
	            if (originalPoint){
		            var dx=touch.keyboardX - originalPoint.x;
		            var dy=touch.keyboardY - originalPoint.y;
		            if (Math.abs(dx)>Math.abs(dy)){
			            if (dx>threshold){
				            touchHandler("swiperight")
			            }else{
				            touchHandler("swipeleft")
			            }
		            }else{
			            if (dy>threshold){
				            touchHandler("swipedown")
			            }else{
				            touchHandler("swipeup")
			            }
		            }
		            //删除事件起点
		            originalPoint=touchenabledkeyboardEvents[touch.identifier]=null;
	            }
            }else if (touch.state=="TouchPressed"){
            	//如果手指碰到键盘，记下当时的位置（X/Y）和时间，这个点是事件的起点
	            touchenabledkeyboardEvents[touch.identifier] = {
		            x:touch.keyboardX,
		            y:touch.keyboardY,
		            t:event.timeStamp
	            }
            }
        }
    )
}
