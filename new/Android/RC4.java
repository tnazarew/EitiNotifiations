package com.example.maksimandrzejewski.first;

import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;

/**
 * Created by Maksim Andrzejewski on 16-May-16.
 */
public class RC4 {

    private byte [] key;
    private Cipher rc4;
    private Cipher rc4Decrypt;
    private SecretKeySpec rc4Key;


    RC4(String k) throws NoSuchPaddingException, NoSuchAlgorithmException, UnsupportedEncodingException, InvalidKeyException {
        key = k.getBytes("ASCII");
        rc4Key = new SecretKeySpec(key, "RC4");
        rc4Decrypt = Cipher.getInstance("RC4");
        rc4Decrypt.init(Cipher.DECRYPT_MODE, rc4Key);
        rc4 = Cipher.getInstance("RC4");
        rc4.init(Cipher.ENCRYPT_MODE, rc4Key);
    }

    public String decrypt(byte[] message) throws InvalidKeyException, UnsupportedEncodingException, NoSuchAlgorithmException, NoSuchPaddingException {

        byte [] clearText = rc4Decrypt.update(message);


        return new String(clearText, "ASCII").substring(0, clearText.length-1);
    }



    public byte[] encrypt(String message) throws InvalidKeyException, UnsupportedEncodingException, NoSuchAlgorithmException, NoSuchPaddingException {
        byte [] cipherText = rc4.update(message.getBytes("ASCII"));
        return cipherText;
    }

    public String keyToString()
    {
        return new String(key);
    }

    public void setNewKey(String k) throws UnsupportedEncodingException, InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException{
        key = k.getBytes("ASCII");
        rc4Key = new SecretKeySpec(key, "RC4");
        rc4Decrypt = Cipher.getInstance("RC4");
        rc4Decrypt.init(Cipher.DECRYPT_MODE, rc4Key);
        rc4 = Cipher.getInstance("RC4");
        rc4.init(Cipher.ENCRYPT_MODE, rc4Key);
    }

}

