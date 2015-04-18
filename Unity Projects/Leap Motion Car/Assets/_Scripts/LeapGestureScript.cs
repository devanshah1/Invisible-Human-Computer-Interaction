using UnityEngine;
using System.Collections;
using Leap;

public class LeapGestureScript : MonoBehaviour {

	Controller leapController;
	public TextMesh screenText;
	private string gestureText;
	public GameObject wiper1;
	public GameObject wiper2; 
	public GameObject sunroof;
	/*private Vector3 origWip1Position;
	private Vector3 origWip2Position; 
	private Vector3 origSunroofPosition;*/

	void Start() {
		gestureText = "Welcome to your Car!";
		SetScreenText();
		leapController = new Controller();
		leapController.EnableGesture(Gesture.GestureType.TYPE_SWIPE);
		leapController.EnableGesture(Gesture.GestureType.TYPE_CIRCLE);
		leapController.EnableGesture(Gesture.GestureType.TYPESCREENTAP);
		/*origWip1Position = wiper1.transform.position;
		origWip2Position = wiper2.transform.position;
		origSunroofPosition = sunroof.transform.position;*/
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

					/*Debug.Log("wip1Position = " + origWip1Position);
					Debug.Log("wip2Position = " + origWip2Position);*/

					if (Mathf.Abs(swipe.Direction.x) > Mathf.Abs(swipe.Direction.y)) { // gesture is horizontal swipe
						if (swipe.Direction.x > 0) { // right swipe
							Debug.Log ("SWR");
							gestureText = "Wipers Off";
							SetScreenText();

							/*wiper1.transform.position = origWip1Position;
							wiper2.transform.position = origWip2Position;*/
						}
						else { // left swipe
							Debug.Log ("SWL");
							gestureText = "Wipers On";
							SetScreenText();

							/*wiper1.transform.RotateAround(origWip1Position, Vector3.up, 100 * Time.deltaTime);
							wiper2.transform.RotateAround(origWip2Position, Vector3.up, 100 * Time.deltaTime);*/
						}
					}
					else { // gesture is vertical swipe
						if (swipe.Direction.y > 0) { // gesture is swipe down
							Debug.Log("SWD");
							gestureText = "Sunroof Closed";
							SetScreenText();

							// sunroof.transform.position = origSunroofPosition;
						}
						else { // gesture is swipe up
							Debug.Log("SWU");
							gestureText = "Sunroof Open";
							SetScreenText();

							// sunroof.transform.position
						}
					}
				    break;

				/*case Gesture.GestureType.TYPESCREENTAP:
					KeyTapGesture keytap = new KeyTapGesture(currGest);
					leapController.Frame(1).Gesture(currGest.Id).Ge
				 	if (
					Debug.Log("Keytap");
					gestureText = "Play Music";*/
			}

		}
	}
	
	void SetScreenText() {
		screenText.text = gestureText;
	}
}
