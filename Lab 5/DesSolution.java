import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import javax.crypto.*;
import java.util.Base64;

// Part 1
public class DesSolution {
    public static void main(String[] args) throws Exception {
        String fileName = "shorttext.txt";
        String data = "";
        String line;
        BufferedReader bufferedReader = new BufferedReader( new FileReader(fileName));
        while((line= bufferedReader.readLine())!=null){
            data = data +"\n" + line;
        }
        // System.out.println("Original content: "+ data);

// generate secret key using DES algorithm
        KeyGenerator keyGen = KeyGenerator.getInstance("DES");
        SecretKey desKey = keyGen.generateKey();
        
// create cipher object, initialize the ciphers with the given key, choose encryption mode as DES
        Cipher desCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
        desCipher.init(Cipher.ENCRYPT_MODE, desKey);
       
// do encryption, by calling method Cipher.doFinal().
        // if plaintext input is string,
        // convert to byte array using getBytes() method before passing to doFinal() 
        byte[] dataBytes = data.getBytes();
        byte[] encryptedBytes = desCipher.doFinal(dataBytes);
        // String base64format = Base64.getEncoder().encodeToString(desCipher);
// print the length of output encrypted byte[], compare the length of file shorttext.txt and longtext.txt
        System.out.println(encryptedBytes.length); 
        // System.out.println(new String(encryptedBytes));

// do format conversion. Turn the encrypted byte[] format into base64format String using Base64
        String encryptedData = Base64.getEncoder().encodeToString(encryptedBytes);

// print the encrypted message (in base64format String format)
        // System.out.println(encryptedData);

// create cipher object, initialize the ciphers with the given key, choose decryption mode as DES
        desCipher.init(Cipher.DECRYPT_MODE, desKey);

// do decryption, by calling method Cipher.doFinal().
        byte[] decryptedBytes = desCipher.doFinal(encryptedBytes);

// do format conversion. Convert the decrypted byte[] to String, using "String a = new String(byte_array);"
        String decryptedData = new String(decryptedBytes);

// print the decrypted String text and compare it with original text
        System.out.println(decryptedData);
        System.out.println(data);
    }
}