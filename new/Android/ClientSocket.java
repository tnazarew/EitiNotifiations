package com.example.maksimandrzejewski.first;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;

import org.json.simple.parser.ParseException;

import javax.crypto.NoSuchPaddingException;

public class ClientSocket {

    private RSA myKey;
    private JSON Json = new JSON();
    private MD5 hashMD5;
    private String messageToHash;
    private byte[] hashMess;

    final String LOG_TAG = "myLogs";
    private Handler handler = new Handler();

    private MainMenu mainMenu;
    private static String mHostIpAddress;
    private static int mPort;

    private Thread mServiceThread;
    private Runnable mServiceRunnable;

    private Socket mSocket;

    private volatile boolean isConnected = false;
    private volatile boolean isTested = true;

    private int mSocketStatus = 0;
   
    private volatile String mOutString = "";

    private volatile boolean mIsNewData;

    private int firstPart;
    private int secondPart;

    private String symKey;
    private RC4 rc4Key;

    public ClientSocket(String pHostIp, int pPort, MainMenu mainMenu) {
        mHostIpAddress = pHostIp;
        mPort = pPort;
        this.mainMenu = mainMenu;

    }

    public void connectRSA(final int pTimeoutMillisecs) {
        if (mSocketStatus != 0) {
            return;
        }

        mServiceRunnable = new Runnable() {
            @Override
            public void run() {
                mSocketStatus = 1; 
                try {
                    mSocket = new Socket();

                        mSocket.connect(new InetSocketAddress(mHostIpAddress, mPort), pTimeoutMillisecs);

                    mSocket.setKeepAlive(true);
                    setConnected(true);
                    onConnect();
                  

                     byte[] byteArray = Header.createInputRSA(getKey());

                    int count = 3;
                        while ( isConnected() && !mServiceThread.currentThread().isInterrupted()) {


                            if(count > 0 && count < 4) {

                                if(count == 3) {
                                  
                                    DataOutputStream dOut = new DataOutputStream(mSocket.getOutputStream());

                                    Log.d(LOG_TAG, String.valueOf(byteArray.length));

                                    dOut.write(byteArray);
                                }


                                Log.d(LOG_TAG, "Wait 0,2 sec");
                                try {
                                    mServiceThread.sleep(200);
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }


                                DataInputStream dIn = new DataInputStream(mSocket.getInputStream());
                                int length = 0;

                                if ((length = dIn.readInt()) != 0 && count != 0) {
                                    if (count == 3) {
                                        Log.d(LOG_TAG, "LENGTH: ");
                                        length = toLittleEndian(length);
                                        Log.d(LOG_TAG, String.valueOf(length));
                                    }
                                    if (count == 2) {
                                        Log.d(LOG_TAG, "FIRST PART: ");
                                        firstPart = toLittleEndian(length);
                                        Log.d(LOG_TAG, String.valueOf(firstPart));
                                    }

                                    if (count == 1) {
                                        Log.d(LOG_TAG, "SECOND PART: ");
                                        secondPart = toLittleEndian(length);
                                        Log.d(LOG_TAG, String.valueOf(secondPart));
                                    }

                                    count--;

                                }

                            }
                            else if(count == 0)
                            {


                                BigInteger firstE = BigInteger.valueOf(getUnsignedInt(firstPart));
                                BigInteger secondE = BigInteger.valueOf(getUnsignedInt(secondPart));
                                BigInteger firstD;
                                BigInteger secondD;
                                firstD = myKey.decrypt(firstE);
                                Log.d(LOG_TAG, "FIRST PART DECRYPTED " + firstD.intValue());
                                secondD = myKey.decrypt(secondE);
                                Log.d(LOG_TAG, "SECOND PART DECRYPTED " + secondD.intValue());

                                byte[] firstArray = firstD.toByteArray();
                                byte[] secondArray = secondD.toByteArray();
                                byte[] message = new byte[firstArray.length + secondArray.length];

                                for(int i = 0; i < message.length; i++)
                                {
                                    if(i < firstArray.length)
                                        message[i] = firstArray[firstArray.length - i - 1];
                                    else message[i] = secondArray[firstArray.length + secondArray.length -i - 1];

                                }

                                symKey = new String(message);

                                System.out.println("Decrypted message: " + symKey );

                                int firstInt = firstD.intValue();
                                int secondInt = secondD.intValue();
                                Log.d(LOG_TAG, "SECOND PART DECRYPTED in STRING " + new String(firstArray));
                                Log.d(LOG_TAG, "SECOND PART DECRYPTED in STRING " + new String(secondArray));

                                rc4Key = new RC4(symKey);
                                break;

                            }
                                }

                        mSocketStatus = 0;
                        mSocket.close();
                        handler.post(new Runnable() {
                            @Override
                            public void run() {
                                onDisconnect();
                              
                            }
                        });


                }  catch (SocketTimeoutException e) {
                    final SocketTimeoutException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                          
                            Log.d(LOG_TAG, "SocketTimeoutException");
                        }
                    });
                    e.printStackTrace();
                } catch (UnknownHostException e) {
                    final UnknownHostException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            mSocketStatus = -1;
                            onSocketError(mSocketStatus,_e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "UnknownHostException");
                        }
                    });
                    e.printStackTrace();
                } catch (SocketException e) {
                    mServiceThread.currentThread().interrupt();
                    final SocketException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "SocketException");

                        }
                    });
                    e.printStackTrace();
                } catch (IOException e) {
                    final IOException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "IOException");
                        }
                    });
                    e.printStackTrace();
                } catch (NoSuchPaddingException e) {
                    e.printStackTrace();
                } catch (NoSuchAlgorithmException e) {
                    e.printStackTrace();
                } catch (InvalidKeyException e) {
                    e.printStackTrace();
                }
            }
        };
        //onDisconnect();
        mServiceThread = new Thread(mServiceRunnable);
        mServiceThread.start();
    }


    // wysylam 0 0 2
    public void connectRC4(final int pTimeoutMillisecs) {
        if (mSocketStatus != 0) {
            return;
        }

        mServiceRunnable = new Runnable() {
            @Override
            public void run() {
                mSocketStatus = 1; 
                try {
                    mSocket = new Socket();

                    mSocket.connect(new InetSocketAddress(mHostIpAddress, mPort), pTimeoutMillisecs);

                    mSocket.setKeepAlive(true);
                    setConnected(true);
                    onConnect();
               

                    byte[] byteArray = Header.createInputRC4();

                    rc4Key.setNewKey(symKey);
                    int count = 3;
                    while ( isConnected() && !mServiceThread.currentThread().isInterrupted()) {



                        DataOutputStream dOut = new DataOutputStream(mSocket.getOutputStream());

                        Log.d(LOG_TAG, String.valueOf(byteArray.length));

                        dOut.write(byteArray);



                            Log.d(LOG_TAG, "Wait 1 sec");
                            try {
                                mServiceThread.sleep(1000);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }


                            int length;
                            DataInputStream dIn = new DataInputStream(mSocket.getInputStream());
                            length = dIn.readInt();
                            length = toLittleEndian(length);
                            byte[] messageRC4E = new byte[length];

                            System.out.println("Length rc4: " + length);

                            dIn.readFully(messageRC4E, 0, messageRC4E.length);

                            System.out.println("RC4 message encrypted: " + messageRC4E);

                            String messageRC4D = rc4Key.decrypt(messageRC4E);

                            System.out.println("RC4 message decrypted: " + messageRC4D);
                            Log.d(LOG_TAG, "RC4 message decrypted: " + messageRC4D);

                            System.out.println("Number of bytes in decrypted rc4: " + messageRC4D.getBytes().length);

                            messageToHash = Json.decodeMessages(messageRC4D);

                            System.out.println("Message: " + messageToHash);

                            hashMD5 = new MD5();
                             hashMess = rc4Key.encrypt(hashMD5.encrypt(messageToHash));
                            System.out.println("Hash Message: " + hashMD5.encrypt(messageToHash));

                        break;


                    }

                    mSocketStatus = 0;
                    mSocket.close();
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onDisconnect();
                           
                        }
                    });


                }  catch (SocketTimeoutException e) {
                    final SocketTimeoutException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                     
                            Log.d(LOG_TAG, "SocketTimeoutException");
                        }
                    });
                    e.printStackTrace();
                } catch (UnknownHostException e) {
                    final UnknownHostException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            mSocketStatus = -1;
                            onSocketError(mSocketStatus,_e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "UnknownHostException");
                        }
                    });
                    e.printStackTrace();
                } catch (SocketException e) {
                    mServiceThread.currentThread().interrupt();
                    final SocketException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "SocketException");

                        }
                    });
                    e.printStackTrace();
                } catch (IOException e) {
                    final IOException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "IOException");
                        }
                    });
                    e.printStackTrace();
                } catch (NoSuchPaddingException e) {
                    e.printStackTrace();
                } catch (NoSuchAlgorithmException e) {
                    e.printStackTrace();
                } catch (InvalidKeyException e) {
                    e.printStackTrace();
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            }
        };
        //onDisconnect();
        mServiceThread = new Thread(mServiceRunnable);
        mServiceThread.start();
    }

    // length 0 3 hashRC4
    public void connectMD5(final int pTimeoutMillisecs) {
        if (mSocketStatus != 0) {
            return;
        }

        mServiceRunnable = new Runnable() {
            @Override
            public void run() {
                mSocketStatus = 1; 
               
                try {
                    mSocket = new Socket();

                    mSocket.connect(new InetSocketAddress(mHostIpAddress, mPort), pTimeoutMillisecs);

                    mSocket.setKeepAlive(true);
                    setConnected(true);
                    onConnect();
              
                    System.out.println("MD5 hash: " + hashMess);
                    byte[] byteArray = Header.createInputMD5(hashMess);
                    rc4Key.setNewKey(symKey);

                    System.out.println(rc4Key.keyToString());


                    int count = 3;
                    while ( isConnected() && !mServiceThread.currentThread().isInterrupted()) {




                        DataOutputStream dOut = new DataOutputStream(mSocket.getOutputStream());
                        dOut.write(byteArray);


                        Log.d(LOG_TAG, "Wait 1 sec");
                        try {
                            mServiceThread.sleep(1000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        DataInputStream dIn = new DataInputStream(mSocket.getInputStream());
                        int lengthIn = dIn.readInt();
                        lengthIn = toLittleEndian(lengthIn);
                        byte[] messageFromServer = new byte[lengthIn];

                        System.out.println("Length of answer: " + lengthIn);

                        dIn.readFully(messageFromServer, 0, messageFromServer.length);



                        System.out.println("RC4 message encrypted: " + messageFromServer);

                        String messageRC4D = rc4Key.decrypt(messageFromServer);

                        System.out.println("RC4 message decrypted: " + messageRC4D);
                      

						System.out.println("Number of bytes in decrypted rc4: " + messageRC4D.getBytes().length);

                        String messFromServer = Json.decodeMessages(messageRC4D);

                        System.out.println("Message: " + messFromServer);
                        
                        break;
                      

                    }

                    mSocketStatus = 0;
                    mSocket.close();
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onDisconnect();
                        
                        }
                    });


                }  catch (SocketTimeoutException e) {
                    final SocketTimeoutException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            //connect(1000);
                            Log.d(LOG_TAG, "SocketTimeoutException");
                        }
                    });
                    e.printStackTrace();
                } catch (UnknownHostException e) {
                    final UnknownHostException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            mSocketStatus = -1;
                            onSocketError(mSocketStatus,_e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "UnknownHostException");
                        }
                    });
                    e.printStackTrace();
                } catch (SocketException e) {
                    mServiceThread.currentThread().interrupt();
                    final SocketException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "SocketException");
                          
                        }
                    });
                    e.printStackTrace();
                } catch (IOException e) {
                    final IOException _e = e;
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            onSocketError(mSocketStatus, _e);
                            mSocketStatus = 0;
                            Log.d(LOG_TAG, "IOException");
                        }
                    });
                    e.printStackTrace();
                } catch (NoSuchAlgorithmException e) {
                    e.printStackTrace();
                } catch (NoSuchPaddingException e) {
                    e.printStackTrace();
                } catch (InvalidKeyException e) {
                    e.printStackTrace();
                }
            }
        };

        mServiceThread = new Thread(mServiceRunnable);
        mServiceThread.start();
    }







    public long getUnsignedInt(int x){
        return x & 0x00000000ffffffffL;
    }



    public byte[] getKey()
    {
        myKey = new RSA();
        BigInteger[] pubAndMod = myKey.generateKey();
        byte[] pubAndModByte = new byte[8];
        byte[] pub = ByteBuffer.allocate(4).putInt(pubAndMod[0].intValue()).array();
        byte[] mod = ByteBuffer.allocate(4).putInt(pubAndMod[1].intValue()).array();
        Log.d(LOG_TAG, String.valueOf(pub.length));
        Log.d(LOG_TAG, String.valueOf(mod.length));

        for(int i = 0; i < pubAndModByte.length; i++){
            if (i < 4)
                pubAndModByte[i] = mod[i];
            if(i > 3)
                pubAndModByte[i] = pub[i-4];
        }

        return pubAndModByte;

    }



    public int toLittleEndian(int n){

        byte temp = 0x00;
        byte[] array = ByteBuffer.allocate(4).putInt(n).array();
        for(int i = 0; i < array.length /2;i++)
        {
            temp = array[i];
            array[i] = array[array.length - 1 - i];
            array[array.length - 1 - i] = temp;
        }
        int value = 0;

        for (int i = 0; i < array.length; i++)
        {
            value = (value << 8) + (array[i] & 0xff);

        }

        return value;
    }


    public void disconnect() throws IOException {


        try{
            mServiceThread.interrupt();
        }catch(Exception e){Log.d(LOG_TAG, "Inrerruption");}
        setConnected(false);

    }



    protected synchronized boolean isConnected(){
        return this.isConnected;
    }

    private synchronized boolean setConnected(boolean pIsConnected){
        this.isConnected = pIsConnected;
        return this.isConnected;
    }

    protected synchronized void send(String pOutString){
        mOutString = pOutString;
        mIsNewData = true;
    }

    private synchronized String getSendData(){
        mIsNewData = false;
        return mOutString;
    }

    private synchronized void clearSendData(){
        if (!mIsNewData)
            mOutString = "";
    }


}