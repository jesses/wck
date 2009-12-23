package misc {

	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;	
	import flash.text.TextField;
	
	/**
	 * A class to record and display frames per second. Can also dynamically set the quality of the movie based on the FPS.
	 */
	public class FPS {
		
		/// last recorded time.
		public var lastTime:uint;
		
		/// Number of frames that have passed since last recorded time.
		public var framesPassed:uint = 0;
		
		/// How many frames to wait until FPS is calculated.
		public var resetAfter:uint = 15;
		
		/// Last recorded FPS.
		public var fps:Number;
		
		/// Text field to display the FPS in.
		public var display:TextField;
		
		/// Should the FPS effect the quality of the movie running?
		public var changeQuality:Boolean = false;
		
		/// Output the FPS to the console?
		public var traceFPS:Boolean = false;
		
		/// Decrement quality when this FPS or lower is reached.
		public var lowFPS:Number = 20;
		
		/// Increment quality when this FPS or higher is reached.
		public var highFPS:Number = 30;
		
		/// Precision of the FPS.
		public var precision:uint = 2;
		
		/// The stage.
		public var stage:Stage;
		
		/**
		 * Start calculating FPS.
		 */
		public function startCalc(s:Stage):void {
			lastTime = getTimer();
			framesPassed = 0;
			stage = s;
			stage.addEventListener(Event.ENTER_FRAME, update, false, 0, true);
		}
		
		/**
		 * Stop calculating FPS.
		 */
		public function stopCalc():void {
			stage.removeEventListener(Event.ENTER_FRAME, update);
		}
		
		/**
		 * Calculate it!
		 */ 
		public function update(e:Event) {
			framesPassed++;
			if(framesPassed == resetAfter) {
				var t2:uint = getTimer();
				fps = ((1000 * framesPassed) / (t2 - lastTime));
				lastTime = t2;
				framesPassed = 0;
				if(display) {
					display.text = fps.toFixed(precision) + ' fps';
				}
				if(changeQuality) {
					if(fps < lowFPS) {
						Util.offsetQuality(stage, -1);
					}
					else if(fps > highFPS) {
						Util.offsetQuality(stage, 1);
					}
				}
				if(traceFPS) {
					trace(fps);
				}
			}
		}
		
	}
}