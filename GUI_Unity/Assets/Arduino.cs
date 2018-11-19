using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using UnityEngine.UI;

public class Arduino : MonoBehaviour {

	SerialPort stream = new SerialPort("COM3", 9600);

	public Text currentNoteDisplay;

	string currentNote = "";
	// Use this for initialization
	void Start () {
		stream.Open();
	}
	
	// Update is called once per frame
	void Update () {
		string currentNote = stream.ReadLine();
		currentNoteDisplay.text = currentNote;
	}
}
