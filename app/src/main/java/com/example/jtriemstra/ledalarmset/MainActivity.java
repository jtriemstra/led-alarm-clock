package com.example.jtriemstra.ledalarmset;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.ParcelUuid;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.Iterator;
import java.util.Set;

public class MainActivity extends Activity {
    final int ACTIVITY_REQUEST_CODE_ENABLE_BT = 1;
    final String TAG = "MainActivity";
    BluetoothAdapter m_objAdapter;
    BluetoothDevice m_objServerDevice;
    BluetoothSocket m_objTransferSocket;

    OutputStreamWriter m_objWriter;
    InputStreamReader m_objReader;
    InputStream m_objReadStream;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG, "onCreate");
    }

    public void onFetch(View v)
    {
        if (m_objReader == null || m_objWriter == null)
        {
            Log.d(TAG, "reader/writer null");
            return;
        }

        try
        {
            m_objWriter.write("f");
            m_objWriter.flush();
            final int BUFFER_SIZE = 16;

            byte[] objBuffer = new byte[BUFFER_SIZE];
            Log.d(TAG, new String(objBuffer, "ASCII"));

            int intActualBytesRead = 0, intTotalBytesRead = 0;
            while (intTotalBytesRead < BUFFER_SIZE)
            {
                intActualBytesRead = m_objReadStream.read(objBuffer, intTotalBytesRead, BUFFER_SIZE - intTotalBytesRead);
                Log.d(TAG, new String(objBuffer, "ASCII"));
                Log.d(TAG, Integer.toString(intActualBytesRead));
                intTotalBytesRead += intActualBytesRead;
            }

            String strFetchedData = new String(objBuffer, "ASCII");
            EditText objCurrentTimeInput = (EditText) findViewById(R.id.current_time_input);
            objCurrentTimeInput.setText(strFetchedData.substring(0, 8));
            EditText objAlarmTimeInput = (EditText) findViewById(R.id.alarm_time_input);
            objAlarmTimeInput.setText(strFetchedData.substring(8, 16));
        }
        catch (Exception ex)
        {
            Log.e(TAG, "error fetching", ex);
        }
    }

    public void onConnect(View v)
    {
        Log.d(TAG, "onConnect");
        if (m_objAdapter == null) m_objAdapter = BluetoothAdapter.getDefaultAdapter();

        if (m_objAdapter == null) {
            // Device does not support Bluetooth
            Log.d(TAG, "Bluetooth adapter is null");
            return;
        }

        Set<BluetoothDevice> objDevices = m_objAdapter.getBondedDevices();
        if (objDevices != null)
        {
            Iterator<BluetoothDevice> objDeviceIterator = objDevices.iterator();
            if (objDeviceIterator != null)
            {
                while (objDeviceIterator.hasNext())
                {
                    BluetoothDevice objCurrentDevice = objDeviceIterator.next();

                    //TODO: genericize this
                    if ("00:18:91:D9:18:69".equals(objCurrentDevice.getAddress())) {

                        ParcelUuid[] ids = objCurrentDevice.getUuids();
                        if (ids != null && ids.length > 0)
                        {
                            try
                            {
                                m_objServerDevice = objCurrentDevice;
                                m_objTransferSocket = m_objServerDevice.createInsecureRfcommSocketToServiceRecord(ids[0].getUuid());

                                if (m_objTransferSocket != null)
                                {
                                    m_objTransferSocket.connect();
                                    m_objWriter = new OutputStreamWriter(m_objTransferSocket.getOutputStream());
                                    m_objReader = new InputStreamReader(m_objTransferSocket.getInputStream());
                                    m_objReadStream = m_objTransferSocket.getInputStream();
                                }
                            }
                            catch (Exception ex)
                            {
                                Log.e(TAG, "error connecting", ex);
                            }
                        }
                        else
                        {
                            Log.d(TAG, "uuid array is null or empty");
                        }
                    }
                    else
                    {
                        Log.d(TAG, "expected MAC address not found");
                    }
                }
            }
            else
            {
                Log.d(TAG, "device iterator is null");
            }
        }
        else
        {
            Log.d(TAG, "objDevices is null");
        }
    }

    private void doConnect()
    {

    }
    
    public void onSave(View v)
    {
        if (m_objReader == null || m_objWriter == null)
        {
            Log.d(TAG, "reader/writer null");
            return;
        }

        try
        {
            m_objWriter.write("s");
            m_objWriter.flush();
            final int BUFFER_SIZE = 16;

            EditText objCurrentTimeInput = (EditText) findViewById(R.id.current_time_input);
            Log.d(TAG, objCurrentTimeInput.getText().toString());
            m_objWriter.write(objCurrentTimeInput.getText().toString());
            EditText objAlarmTimeInput = (EditText) findViewById(R.id.alarm_time_input);
            m_objWriter.write(objAlarmTimeInput.getText().toString());
        }
        catch (Exception ex)
        {
            Log.e(TAG, "error saving", ex);
        }
    }

    public void onSaveDebug(View v) throws Exception
    {

        Log.d(TAG, "save clicked");
        if (m_objAdapter == null) m_objAdapter = BluetoothAdapter.getDefaultAdapter();

        if (m_objAdapter == null) {
            // Device does not support Bluetooth
            Log.d("MainActivity", "Bluetooth adapter is null");
            return;
        }

        Log.d(TAG, m_objAdapter.getAddress());
        Log.d(TAG, m_objAdapter.getName());

        Set<BluetoothDevice> objDevices = m_objAdapter.getBondedDevices();
        if (objDevices != null)
        {
            Log.d("", "objDevices not null");
            Iterator<BluetoothDevice> objDeviceIterator = objDevices.iterator();
            if (objDeviceIterator != null)
            {
                Log.d("", "iterator not null");
                while (objDeviceIterator.hasNext())
                {
                    BluetoothDevice objCurrentDevice = objDeviceIterator.next();
                    Log.d(TAG, objCurrentDevice.getAddress());
                    Log.d(TAG, objCurrentDevice.getName());
                    ParcelUuid[] ids = objCurrentDevice.getUuids();
                    if (ids != null && ids.length > 0)
                    {
                        Log.d(TAG, ids[0].getUuid().toString());
                    }
                    else
                    {
                        Log.d(TAG, "uuid array is null or empty");
                    }
                }
            }
        }
        else
        {
            Log.d("", "objDevices is null");
        }

    }
}
