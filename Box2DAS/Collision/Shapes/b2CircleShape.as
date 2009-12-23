package Box2DAS.Collision.Shapes {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	
	/// A circle shape.
	public class b2CircleShape extends b2Shape {
	
		public function b2CircleShape(p:int = 0) {
			_ptr = ((p == 0) ? lib.b2CircleShape_new() : p);
			m_p = new b2Vec2(_ptr + 12);
		}
		
		public function destroy():void {
			lib.b2CircleShape_delete(_ptr);
		}

		/// Implement b2Shape.
		/// bool TestPoint(const b2Transform& transform, const b2Vec2& p) const;
		public override function TestPoint(transform:XF, p:V2):Boolean {
			var center:V2 = transform.p.clone().add(transform.r.multiplyV(m_p.v2));
			var d:V2 = p.clone().subtract(center);
			return (d.dot(d) <= (m_radius * m_radius));
		}
	
		/// Implement b2Shape.
		/// bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input, const b2Transform& transform) const;
		public override function RayCast(output:*, input:*, transform:XF):Boolean {
			/// NOT IMPLEMENTED.
			return false;
		}	
	
		/// @see b2Shape::ComputeAABB
		/// void ComputeAABB(b2AABB* aabb, const b2Transform& transform) const;
		public override function ComputeAABB(aabb:AABB, xf:XF):void {
			/// NOT IMPLEMENTED.
		}
	
		/// @see b2Shape::ComputeMass
		/// void ComputeMass(b2MassData* massData, float32 density) const;
		public override function ComputeMass(massData:b2MassData, density:Number):void {
			/// NOT IMPLEMENTED.
		}

		public var m_p:b2Vec2;
	
	}
}