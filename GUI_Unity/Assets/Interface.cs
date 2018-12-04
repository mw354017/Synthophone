using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Interface : MonoBehaviour {

	public Text currentNoteText;
	
	public Text breathLevelText;

	public Text transposeText;

	wrmhl myDevice = new wrmhl(); // wrmhl is the bridge beetwen your computer and hardware.

	[Tooltip("SerialPort of your device.")]
	public string portName = "COM4";

	[Tooltip("Baudrate")]
	public int baudRate = 9600;


	[Tooltip("Timeout")]
	public int ReadTimeout = 20;

	[Tooltip("QueueLenght")]
	public int QueueLenght = 1;

	[Tooltip("Transpose")]
	public int transpose = 0;

	[Tooltip("Current Note")]
	public string currentNote = "N/A";

	[Tooltip("Breath Level")]
	public int breathLevel = -1;



	private string inputText;

	void Start () {
		myDevice.set (portName, baudRate, ReadTimeout, QueueLenght); // This method set the communication with the following vars;
		myDevice.connect (); // This method open the Serial communication with the vars previously given.
	}

	// Update is called once per frame
	void Update () {
		inputText = myDevice.readQueue(); // myDevice.read() return the data coming from the device using thread.
		string[] words = inputText.Split(' ');
		switch (words[0])
		{
			case "note":
				currentNote = words[1];
				break;
			case "breath":
				breathLevel = int.Parse(words[1]);
				break;
			default:
				// do nothing
				break;
		}
		transposeText.text = transpose.ToString();
		currentNoteText.text = currentNote;
		breathLevelText.text = breathLevel.ToString();
	}

	public void increase_transpose(){
		transpose++;
		myDevice.send(transpose.ToString());
	}

	public void decrease_transpose(){
		transpose--;
		myDevice.send(transpose.ToString());
	}

	void OnApplicationQuit() { // close the Thread and Serial Port
		myDevice.close();
	}
}
