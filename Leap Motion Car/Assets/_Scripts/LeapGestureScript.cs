using UnityEngine;
using System.Collections;
using Leap;

public class LeapGestureScript : MonoBehaviour {

	Controller leapController;
	public TextMesh screenText;
	private string gestureText;

	void Start() {
		gestureText = "Welcome to your Car!";
		SetScreenText();
		leapController = new Controller();
		leapController.EnableGesture(Gesture.GestureType.TYPE_SWIPE);
		leapController.EnableGesture(Gesture.GestureType.TYPE_CIRCLE);
	}

	void Update() {
		Frame frame = leapController.Frame();

		GestureList gestures = frame.Gestures();

		for (int i = 0; i < frame.Gestures().Count; i++) {
			Gesture currGest = gestures[i];

			switch (currGest.Type) {

				case Gesture.GestureType.TYPECIRCLE:
					CircleGesture circle = new CircleGesture(currGest);
					if (circle.Pointable.Direction.AngleTo(circle.Normal) <= Mathf.PI/2) {//&& currGest.State.ToString() == "STATE_END") { // finger clockwise
						Debug.Log("CW");
						gestureText = "Volume Up";
						SetScreenText();
					}
					else { //if (currGest.State.ToString() == "STATE_END") { // finger counter-clockwise
						Debug.Log("CCW");
						gestureText = "Volume Down";
						SetScreenText();
					}
					break;
			
				case Gesture.GestureType.TYPESWIPE:
					SwipeGesture swipe = new SwipeGesture(currGest);
					if (Mathf.Abs(swipe.Direction.x) > Mathf.Abs(swipe.Direction.y)) { // gesture is horizontal swipe
						if (swipe.Direction.x > 0) { // right swipe
							Debug.Log ("SWR");
							gestureText = "Wipers Off";
							SetScreenText();
						}
						else { // left swipe
							Debug.Log ("SWL");
							gestureText = "Wipers On";
							SetScreenText();
						}
					}
					else { // gesture is vertical swipe
						if (swipe.Direction.y > 0) { // gesture is swipe down
							Debug.Log("SWD");
							gestureText = "Sunroof Closed";
							SetScreenText();
						}
						else { // gesture is swipe up
							Debug.Log("SWU");
							gestureText = "Sunroof Open";
							SetScreenText();
						}
					}
				    break;
			}

		}
	}
	
	void SetScreenText() {
		screenText.text = gestureText;
	}
}
