package misc {

	import misc.*;
	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;
	
	public class FXContainer extends FX {
				
		public override function isDone():Boolean {
			return numChildren == 0;
		}
	}
}