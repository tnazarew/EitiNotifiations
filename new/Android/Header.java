package com.example.maksimandrzejewski.first;

import java.nio.ByteBuffer;

/**
 * Created by Maksim Andrzejewski on 20-May-16.
 */

    import java.nio.ByteBuffer;
    import java.nio.charset.Charset;

    import javax.crypto.Cipher;
    import javax.crypto.KeyGenerator;
    import javax.crypto.spec.IvParameterSpec;
    import javax.crypto.spec.SecretKeySpec;

    public class Header
    {
        public static String toStr(int n)
        {
            byte[] byteArray = ByteBuffer.allocate(4).putInt(n).array();
            for(int i=0; i<byteArray.length;i++){
                System.out.print(Integer.toBinaryString(0x100 + byteArray[i]).substring(1) + ' ');
            }
            System.out.println();
            return new String(byteArray);
        }

        public static int toInt(String str)
        {
            byte[] byteArray = str.getBytes();
            int value = 0;
            for (int i = 0; i < byteArray.length; i++)
            {
                value = (value << 8) + (byteArray[i] & 0xff);
            }

            for(int i=0; i<byteArray.length;i++){
                System.out.print(Integer.toBinaryString(0x100 + byteArray[i]).substring(1) + ' ');
            }
            System.out.println();
           
            return value;
        }

        public static byte[] createInputRSA(byte[] byteAr)
        {
            int length = 8;
            int device_id = 1;
            int action = 1;

            byte[] lengthArray = ByteBuffer.allocate(4).putInt(length).array();
            byte[] device_idArray = ByteBuffer.allocate(4).putInt(device_id).array();
            byte[] actionArray = ByteBuffer.allocate(4).putInt(action).array();

            byte[] finalArray = new byte[20];

            for(int i = 0; i < finalArray.length; i++)
            {
                if(i < 4)
                    finalArray[i] = lengthArray[3-i];
                if(i > 3 && i < 8)
                    finalArray[i] = device_idArray[7-i];
                if(i > 7 && i < 12)
                    finalArray[i] = actionArray[11-i];
                if(i > 11)
                    finalArray[i] = byteAr[19-i];
            }

            return finalArray;
        }


        public static byte[] createInputRC4()
        {
            int length = 0;
            int device_id = 0;
            int action = 2;

            byte[] lengthArray = ByteBuffer.allocate(4).putInt(length).array();
            byte[] device_idArray = ByteBuffer.allocate(4).putInt(device_id).array();
            byte[] actionArray = ByteBuffer.allocate(4).putInt(action).array();

            byte[] finalArray = new byte[12];

            for(int i = 0; i < finalArray.length; i++)
            {
                if(i < 4)
                    finalArray[i] = lengthArray[3-i];
                if(i > 3 && i < 8)
                    finalArray[i] = device_idArray[7-i];
                if(i > 7 && i < 12)
                    finalArray[i] = actionArray[11-i];
            }

            return finalArray;
        }


        public static byte[] createInputMD5(byte[] hash)
        {

            int length = hash.length;
            int device_id = 0;
            int action = 3;

            byte[] lengthArray = ByteBuffer.allocate(4).putInt(length).array();
            byte[] device_idArray = ByteBuffer.allocate(4).putInt(device_id).array();
            byte[] actionArray = ByteBuffer.allocate(4).putInt(action).array();

            byte[] finalArray = new byte[12 + hash.length];

            for(int i = 0; i < finalArray.length ; i++)
            {
                if(i < 4)
                    finalArray[i] = lengthArray[3-i];
                if(i > 3 && i < 8)
                    finalArray[i] = device_idArray[7-i];
                if(i > 7 && i < 12)
                    finalArray[i] = actionArray[11-i];
                if(i > 11)
                    finalArray[i] = hash[i-12];
            }

            return finalArray;
        }

    }



