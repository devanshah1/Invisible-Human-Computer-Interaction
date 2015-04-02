using UnityEngine;
using System.Collections;
using Leap;

public class LeapCameraMovement : MonoBehaviour {

	Controller leapController;

	// initialization
	void Start () {
		leapController = new Controller();
	}
	
	// LateUpdate is called after all Updates(); used for cameras
	void LateUpdate () {
		transform.LookAt(Vector3.zero); // look at the skull every frame
		Frame frame = leapController.Frame();

		float rotateY; // move hand vertically
		float rotateX; // move hand horizontally
		float zoom; // move hand on z
		float grabStrength; // detects grab gesture to activate camera
		Vector3 xAxis = Vector3.right;
		Vector3 yAxis = Vector3.up;

		HandList hands = frame.Hands;
		for (int i = 0; i < frame.Hands.Count; i++) {
			Hand hand = hands[0]; // first hand in the frame
			grabStrength = hand.GrabStrength;
			if (grabStrength != 1.0f) { break; /* do nothing if grab is not detected */ }
			else {
				Vector handSpeed = hand.PalmVelocity;
				rotateX = (handSpeed.x)/100.0f;
				Debug.Log("X: " + rotateX);
				rotateY = (handSpeed.y)/100.0f;
				Debug.Log("Y: " + rotateY);
				zoom = ((handSpeed.z)/1000.0f)*-1.0f;
				Debug.Log("Z: " + zoom);

				transform.RotateAround(Vector3.zero, yAxis, rotateX); // rotate around y-axis
				transform.RotateAround(Vector3.zero, xAxis, rotateY); // rotate around x-axis
				transform.position = Vector3.MoveTowards(transform.position, Vector3.zero, zoom); // zoom towards (0,0,0)
			}
 		}
	}

}
