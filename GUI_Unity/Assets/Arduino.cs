using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using UnityEngine.UI;

public class Arduino : MonoBehaviour {

	public Text test_text;

	public string inputText;

	SerialPort stream = new SerialPort("COM4", 9600);


	// Use this for initialization
	void Start () {
		Debug.Log("Start");
		OpenConnection();
	}

	public void OpenConnection() 
    {
		Debug.Log("Open COnnection");
       if (stream != null) 
       {
         if (stream.IsOpen) 
         {
          stream.Close();
          test_text.text = ("Closing port, because it was already open!");
         }
         else 
         {
          stream.Open();  // opens the connection
          stream.ReadTimeout = 16;  // sets the timeout value before reporting error
          test_text.text = ("Port Opened!");
		//		message = "Port Opened!";
         }
       }
       else 
       {
         if (stream.IsOpen)
         {
          test_text.text = ("Port is already open");
         }
         else 
         {
          test_text.text = ("Port == null");
         }
       }
    }
	
	// Update is called once per frame
	void Update () {
		inputText = stream.ReadLine();
		test_text.text = inputText;
	}

	public void buttonPress() {
		stream.Write("Hello World");
	}

	void OnApplicationQuit() 
    {
       stream.Close();
    }
}
