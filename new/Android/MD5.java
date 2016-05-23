package com.example.maksimandrzejewski.first;

/**
 * Created by Maksim Andrzejewski on 16-May-16.
 */

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MD5 {
        private MessageDigest md;

     MD5() throws NoSuchAlgorithmException {
        //MessageDigest - haszujaca klasas, parametr MD5 odpowiada za rodzaj haszowania
            md = MessageDigest.getInstance("MD5");
        }

    public String encrypt(String message)
        {

        md.update(message.getBytes());
        //Digest - zwraca tablice bajtow reprezantujaca MD5 hasz
        byte[] digest = md.digest();
        StringBuffer sb = new StringBuffer();
        for (byte b : digest) {
            sb.append(String.format("%02x", b & 0xff));
        }

            return sb.toString();
        }


    }
