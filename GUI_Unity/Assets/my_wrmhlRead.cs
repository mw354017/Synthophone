using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class my_wrmhlRead : MonoBehaviour {


	public Text currentNote;
	
	public Text breathLevel;

	public Text test_text;

	public InputField comm;
	
	public InputField baud;



	wrmhl myDevice = new wrmhl(); // wrmhl is the bridge beetwen your computer and hardware.

	[Tooltip("SerialPort of your device.")]
	public string portName = "COM4";

	[Tooltip("Baudrate")]
	public int baudRate = 9600;


	[Tooltip("Timeout")]
	public int ReadTimeout = 20;

	[Tooltip("QueueLenght")]
	public int QueueLenght = 1;

	private string inputText;

	void Start () {
		myDevice.set (portName, baudRate, ReadTimeout, QueueLenght); // This method set the communication with the following vars;
		//                              Serial Port, Baud Rates, Read Timeout and QueueLenght.
		myDevice.connect (); // This method open the Serial communication with the vars previously given.
	}

	// Update is called once per frame
	void Update () {
		inputText = myDevice.readQueue(); // myDevice.read() return the data coming from the device using thread.
		test_text.text = inputText;
		string[] words = inputText.Split(' ');
		switch (words[0])
		{
			case "note":
				currentNote.text = words[1];
				break;
			case "breath":
				breathLevel.text = words[1];
				break;
			default:
				// do nothing
				break;
		}
	}

	void OnApplicationQuit() { // close the Thread and Serial Port
		myDevice.close();
	}

	void setCOMMandBAUD() {
		portName = comm.text;
		baudRate = int.Parse(baud.text);
		Start();
	}
}
