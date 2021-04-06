import java.util.Base64;
import javax.crypto.Cipher;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.*;

// Part 3
public class DigitalSignatureSolution {

    public static void main(String[] args) throws Exception {
//Read the text file and save to String data
        String fileName = "longtext.txt";
        String data = "";
        String line;
        BufferedReader bufferedReader = new BufferedReader( new FileReader(fileName));
        while((line= bufferedReader.readLine())!=null){
            data = data +"\n" + line;
        }
        // System.out.println("Original content: "+ data);

// generate a RSA keypair, initialize as 1024 bits, get public key and private key from this keypair.
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA"); 
        keyGen.initialize(1024); 
        KeyPair keyPair = keyGen.generateKeyPair(); 
        Key publicKey = keyPair.getPublic(); 
        Key privateKey = keyPair.getPrivate();


// Calculate message digest, using MD5 hash function
        MessageDigest md = MessageDigest.getInstance("MD5");
        byte[] dataBytes = data.getBytes();
        md.update(dataBytes);
        byte[] dataDigest = md.digest();

// print the length of output digest byte[], compare the length of file shorttext.txt and longtext.txt
        System.out.println(dataDigest.length);
        String base64dataDigest = Base64.getEncoder().encodeToString(dataDigest);
        System.out.println(base64dataDigest);
           
// Create RSA("RSA/ECB/PKCS1Padding") cipher object and initialize is as encrypt mode, use PRIVATE key.
        Cipher rsaCipher = Cipher.getInstance("RSA/ECB/PKCS1Padding"); 
        rsaCipher.init(Cipher.ENCRYPT_MODE, privateKey);

// encrypt digest message
        byte[] encryptedDigest = rsaCipher.doFinal(dataDigest);

// print the encrypted message (in base64format String using Base64) 
        String base64EncryptedDigest = Base64.getEncoder().encodeToString(encryptedDigest);
        System.out.println(base64EncryptedDigest);

// Create RSA("RSA/ECB/PKCS1Padding") cipher object and initialize is as decrypt mode, use PUBLIC key. 
        rsaCipher.init(Cipher.DECRYPT_MODE, publicKey);          

// decrypt message
        byte[] decryptedDigest = rsaCipher.doFinal(encryptedDigest);

// print the decrypted message (in base64format String using Base64), compare with origin digest 
        String base64DecryptedDigest = Base64.getEncoder().encodeToString(decryptedDigest);
        System.out.println(base64DecryptedDigest);

    }

}