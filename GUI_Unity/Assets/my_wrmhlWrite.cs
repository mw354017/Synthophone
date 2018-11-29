using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class my_wrmhlWrite : MonoBehaviour {

	wrmhl myDevice = new wrmhl(); // wrmhl is the bridge beetwen your computer and hardware.

	[Tooltip("SerialPort of your device.")]
	public string portName = "COM8";

	[Tooltip("Baudrate")]
	public int baudRate = 9600;


	[Tooltip("Timeout")]
	public int ReadTimeout = 20;

	[Tooltip("Something you want to send.")]
	public string dataToSend = "Hello World!";

	[Tooltip("QueueLenght")]
	public int QueueLenght = 1;


	void Start () {
		myDevice.set (portName, baudRate, ReadTimeout, QueueLenght); // This method set the communication with the following vars;
		//                              Serial Port, Baud Rates, Read Timeout and QueueLenght.
		myDevice.connect (); // This method open the Serial communication with the vars previously given.
	}

	// Update is called once per frame
	void Update () {

	}
	

	void OnApplicationQuit() { // close the Thread and Serial Port
		myDevice.close();
	}

	public void buttonPress() {
		myDevice.send(dataToSend); // Send data to the device using thread.
	}
}
