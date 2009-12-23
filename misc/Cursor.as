package misc {

	import misc.*;
	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;
	import flash.ui.*;
	
	public class Cursor extends Entity {
		
		public override function create():void {
			mouseEnabled = false;
			mouseChildren = false;
			Mouse.hide();
			super.create();
			listenWhileVisible(stage, Event.ENTER_FRAME, handleEnterFrame);
		}
		
		public function handleEnterFrame(e:Event):void {
			Util.setPos(this, parent.globalToLocal(Input.mousePos));
		}
	}
}