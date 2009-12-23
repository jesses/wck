package misc {

	import misc.*;
	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;
	
	public class FX extends MovieClip {
		
		public var onDone:Function = null;
		
		public function FX() {
			mouseEnabled = false;
			mouseChildren = false;
			addEventListener(Event.ENTER_FRAME, checkDone);
		}
		
		public function checkDone(e:Event):void {
			if(isDone()) {
				Util.remove(this);
				removeEventListener(Event.ENTER_FRAME, checkDone);
				if(onDone != null) {
					onDone();
				}
			}
		}
		
		public function isDone():Boolean {
			return (currentFrame == totalFrames);
		}
	}
}