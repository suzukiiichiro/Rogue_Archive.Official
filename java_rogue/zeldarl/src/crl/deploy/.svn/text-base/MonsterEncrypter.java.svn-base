package crl.deploy;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import sz.crypt.DESEncrypter;

public class MonsterEncrypter {
	public static void main(String[] args){
		try {
			System.out.println("Writing encrypted file!");
			DESEncrypter encrypter = new DESEncrypter("If you can see this, you are the one to go ahead and get the monsters info");
			encrypter.encrypt(new FileInputStream("data/monsters.xml"), new FileOutputStream("data/monsters.exml"));
			System.out.println("File written");
		} catch (IOException ioe){
			ioe.printStackTrace();
		}
	}}
