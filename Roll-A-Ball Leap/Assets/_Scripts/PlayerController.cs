using UnityEngine;
using System.Collections;
using Leap;

public class PlayerController : MonoBehaviour {
	public float speed;
	public GUIText countText;
	public GUIText winText;
	private int count;
	Controller leapController;

	void Start() {
		count = 0;
		SetCountText();
		winText.text = "";
		leapController = new Controller();
	}

	void FixedUpdate() { // sphere movement with Leap
		Frame frame = leapController.Frame ();
		float moveHorizontal = Input.GetAxis("Horizontal"); //= 0.0f;
		float moveVertical = Input.GetAxis("Vertical"); //= 0.0f; 

		HandList hands = frame.Hands;
		for (int i = 0; i < frame.Hands.Count; i++) {
			Hand hand = hands[i];
			Vector handSpeed = hand.PalmVelocity;

			moveHorizontal = (handSpeed.x)/100.0f;
			Debug.Log(moveHorizontal);
			moveVertical = ((handSpeed.z)/100.0f)*(-1.0f);
			Debug.Log(moveVertical);

			Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);
			GetComponent<Rigidbody>().AddForce(movement * speed * Time.deltaTime);
		}
	}

	void OnTriggerEnter(Collider other) {
		if (other.gameObject.tag == "PickUp") {
			other.gameObject.SetActive(false);
			count = count + 1;
			SetCountText();
		}
	}

	void SetCountText() {
		countText.text = "Count: " + count.ToString();
		if (count >= 10) {
			winText.text = "You Win!";
		}
	}
}