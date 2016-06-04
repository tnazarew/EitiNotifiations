package com.example.maksimandrzejewski.first;

/**
 * Created by Maksim Andrzejewski on 16-May-16.
 */

import android.util.Log;

import java.math.BigInteger;
import java.security.SecureRandom;


public class RSA {
    private BigInteger one      = new BigInteger("1");
    private SecureRandom random = new SecureRandom();

    private BigInteger privateKey;
    private BigInteger publicKey;
    private BigInteger modulus;
    private BigInteger ServerpublicKey;
    private BigInteger Servermodulus;
    final String LOG_TAG = "RSAlogs";


    RSA() {

    }

    RSA(String key, String mod){
        ServerpublicKey = new BigInteger(key);
        Servermodulus = new BigInteger(mod);
    }


    public String[] encryptPublicKeyAndModulus()
    {

        BigInteger[] pubAndMod = generateKey();
        String[] messageToEncrypt = {new String(pubAndMod[0].toByteArray()), new String(pubAndMod[1].toByteArray())};

        System.out.println("Public Key to encrypt: " + new BigInteger(messageToEncrypt[0].getBytes()));
        System.out.println("Modulus to encrypt: " + new BigInteger(messageToEncrypt[1].getBytes()));

        BigInteger encryptPub = encrypt(pubAndMod[0]);
        BigInteger encryptMod = encrypt(pubAndMod[1]);

        String[] encryptedArray = {new String(encryptPub.toByteArray()), new String(encryptMod.toByteArray()),new String(pubAndMod[1].toByteArray()) };
        return encryptedArray;
    }

    public BigInteger[] generateKey()
    {
        int N = 32;
        BigInteger p = BigInteger.probablePrime(N/2, random);
        BigInteger q = BigInteger.probablePrime(N/2, random);
        BigInteger phi = (p.subtract(one)).multiply(q.subtract(one));
        modulus   = p.multiply(q);
        publicKey  = new BigInteger("119");
        privateKey = publicKey.modInverse(phi);
        Log.d(LOG_TAG, publicKey.toString());
        Log.d(LOG_TAG, modulus.toString());
        return new BigInteger[] {publicKey, modulus};

    }

    BigInteger encryptServer(BigInteger message) {
        return message.modPow(ServerpublicKey, Servermodulus);
    }

    BigInteger encrypt(BigInteger message) {
        return message.modPow(publicKey, modulus);
    }



    BigInteger decrypt(BigInteger encrypted) {

        return encrypted.modPow(privateKey, modulus);
    }

    BigInteger decryptTest(BigInteger encrypted,BigInteger priv , BigInteger mod) {
        return encrypted.modPow(priv, mod);
    }

    public String toString() {
        String s = "";
        s += "public  = " + publicKey  + "\n";
        s += "private = " + privateKey + "\n";
        s += "modulus = " + modulus;
        return s;
    }
}
