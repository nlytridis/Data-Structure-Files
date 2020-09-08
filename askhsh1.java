/********************** Nikos Lytridis 2009030088 ********************/

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.RandomAccessFile;
import java.util.Random;

public class askhsh1 {

	private int numberOfInts = (int) Math.pow(10, 7);
	private int[] buffer = new int[256];
	private int n = numberOfInts / 256;
	private int numberOfloops = numberOfInts / buffer.length - 1;
	static final int eightKBytes = 8192; // 8Kbyts
	private int epilogh;
	int rand;
	BufferedReader stdinp = new BufferedReader(new InputStreamReader(System.in));

	public static void main(String args[]) throws FileNotFoundException,
			IOException, ClassNotFoundException, InterruptedException {
		askhsh1 ask = new askhsh1();

	}

	public askhsh1() throws FileNotFoundException, IOException,
			ClassNotFoundException, InterruptedException {
		do {
			System.out.println("Dose thn epilogh sou");
			System.out.println("1 gia dhmiourgeia toy arxeiou");
			System.out.println("2 gia anagnosh tou arxeiou");
			System.out.println("3 gia siriakh anazhthsh sto arxeiou");
			System.out.println("4 gia diadikh anazhthsh sto arxeiou");
			System.out.println("5 gia diadikh anazhthsh paremvolhs sto arxeiou");
			System.out.println("6 gia EXIT");
			
			String inputt = stdinp.readLine();
			epilogh = Integer.parseInt(inputt);
			
			if (epilogh == 1) {
				CreateFile();
			} else if (epilogh == 2) {
				ReadFile();
			} else if (epilogh == 3) {
				System.out.println("Dose to tyxaio klidi");
				String input = stdinp.readLine();
				rand = Integer.parseInt(input);
				System.out.println("zhthsame ton ari8mo " + rand
						+ "kai xreiasthke " + SerialSearch(rand)
						+ "disk accesses");

			} else if (epilogh == 4) {
				System.out.println("Dose to tyxaio klidi");
				String input = stdinp.readLine();
				rand = Integer.parseInt(input);
				System.out.println("zhthsame ton ari8mo " + rand
						+ "kai xreiasthke " + BinarySearch(rand)
						+ "disk accesses");

			} else if (epilogh == 5) {
				System.out.println("Dose to tyxaio klidi");
				String input = stdinp.readLine();
				rand = Integer.parseInt(input);
				System.out.println("zhthsame ton ari8mo " + rand
						+ "kai xreiasthke " + InterpolationSearch(rand)
						+ "disk accesses");

			}else if (epilogh == 6) {
				System.out.println("Telos!!!!!");
			}

		} while (epilogh != 6);
	}

	public void CreateFile() throws IOException, InterruptedException {
		RandomAccessFile MyFile = new RandomAccessFile("SortedFile.dat", "rw");
		ByteArrayOutputStream bos = new ByteArrayOutputStream(1024);
		DataOutputStream out = new DataOutputStream(bos);
		byte[] buf;
		int j = 0;
		for (int i = 0; i < numberOfInts; i++) {
			buffer[j] = i;
			j++;
			if (j == 256 || i == numberOfInts - 1) {
				//System.out.println(i);
				 Thread.sleep(100);
				// System.out.println("gemise o buffer h teleiose to metrhma");
				for (int k = 0; k < j; k++) {
					MyFile.writeInt(buffer[k]);
					buffer[k] = 0;
				}
				 //System.out.println("MetatrophSeByteArrey");
				 buf= bos.toByteArray();
				 //System.out.println("Grafw to arxeio");
				 MyFile.write(buf);
				 //System.out.println("Arxeio Grafthke");
				j = 0;
			}
		}
		out.flush();
		out.close();
		MyFile.close();
	}

	public void ReadFile() throws FileNotFoundException, IOException,
			ClassNotFoundException {
		RandomAccessFile raf = new RandomAccessFile("SortedFile.dat", "r");
		for (int i = 0; i < numberOfInts; i++) {
			System.out.println(raf.readInt());
		}
		raf.close();
	}

	public int SerialSearch(int searchNumber) throws FileNotFoundException,
			IOException, InterruptedException {
		int[] pageFile = new int[256];
		int numberOfAccesses = 0;
		RandomAccessFile raf = new RandomAccessFile("SortedFile.dat", "r");
		System.out.println(searchNumber);
		Thread.sleep(2000);
		while (raf.getFilePointer() != raf.length()) {
			for (int i = 0; i < 256; i++) {
				if (raf.getFilePointer() != raf.length()) {
					pageFile[i] = raf.readInt();
					System.out.println(pageFile[i]);
				} else {
					break;
				}
			}

			numberOfAccesses++;
			System.out.println("Num of accesses " + numberOfAccesses);
			for (int i = 0; i < pageFile.length; i++) {
				if (searchNumber == pageFile[i]) {
					System.out.println("Bre8hke to noumero " + searchNumber);
					raf.close();
					return numberOfAccesses;
				}
			}
		}
		System.out.println("den bre8hke to noumero");
		raf.close();
		return numberOfAccesses;
	}

	public int BinarySearch(int searchNumber) throws FileNotFoundException,
			IOException {
		RandomAccessFile raf = new RandomAccessFile("SortedFile.dat", "r");
		// orizw metablhtes
		int keli = 0;
		int flag = 0;
		int number_of_accesses = 0;
		// ta c kai d einai ta akra tou ka8e pageFile
		long c = 0, d = 0;
		// metablhtes pou xrhsimopoiountai gia thn metakinhsh tou pointer tou
		// raf.
		int seeker_c;
		int seeker_d;
		long length = raf.length();
		// a kai b o ari8mos tou ka8e stoixeiou tou pageFile.
		int a = 1;
		int currNumber;
		float b2 = length / eightKBytes;
		int b = (int) Math.ceil(b2);
		// System.out.println("a =" + a + " b = " + b);
		int temp = 1;
		// oso to tmp einai 1
		while (temp == 1) {
			// auksanw ton ari8mo twn accesses
			number_of_accesses++;
			// to keli einai iso me to a8roisma twn akrwn dia duo.
			keli = Math.round((float) Math.ceil((a + b) / 2));
			// o seeker c mas boh8aei na broume ton pointer pou 8a mas paei sto
			// keli pou 8eloume na elenksoume ta akra tou
			seeker_c = (keli - 1) * eightKBytes;
			// metakinoume ton pointer mexri ekei pou mporei na paei.
			c = raf.skipBytes(seeker_c);
			// meta metakinoume ton pointer sto c.
			raf.seek(c);
			// kai ekxoroume sto c ton int pou diabazei ekeinh thn stigmh.
			c = raf.readInt();
			// omoia metakinoume to d sto telos ths twrinis selidas kai ekxwrome
			// sto d ton akeraio pou mporoume na diabasoume
			seeker_d = (keli - 1) * eightKBytes + eightKBytes - 4;
			d = raf.skipBytes(seeker_d);
			raf.seek(d);
			d = raf.readInt();
			// ean to noumero pou psaxnoume eiani mikrotero apo to c tote to B
			// einai iso me to keli to opoio exoume
			// epileksei twra
			if (searchNumber < c) {
				b = keli;

			}
			// alliws ean einai megalutero to a ginete to keli sto opoio
			// briskomaste twra.
			else if (searchNumber > d) {
				a = keli;

			}
			// alliws to noumero mas einai se auth thn selida kai kanoume
			// anazhthsh.
			else if (searchNumber <= d && searchNumber >= c) {
				raf.seek(seeker_d);
				long endPointer = raf.getFilePointer();
				raf.seek(seeker_c);
				while (raf.getFilePointer() != endPointer) {
					currNumber = raf.readInt();
					if (currNumber == searchNumber) {
						System.out.println("Number found");
						temp = 0;
					} else if (currNumber > searchNumber) {
						flag = 1;
						temp = 0;
					}
				}
			}
		}
		if (flag == 1)
			System.out.println("Number not found");
		System.out
				.println("Number of accesses in disk = " + number_of_accesses);
		return number_of_accesses;
	}

	public int InterpolationSearch(int searchNumber) throws IOException,
			InterruptedException {
		int numberOfAccesses = 1;
		System.out.println("search number= " + searchNumber
				+ " number of ints " + numberOfInts + " n = " + n);
		double page = searchNumber - 1;
		page = page * n;
		page = page / (10000000 - 1);

		int h = (int) Math.floor(page);
		System.out.println(h);

		Thread.sleep(2000);
		RandomAccessFile raf = new RandomAccessFile("SortedFile.dat", "r");
		int[] pageFile = new int[256];
		System.out.println("pointer = " + h * 1024);
		raf.seek(h * 1024);
		for (int i = 0; i < 256; i++) {
			pageFile[i] = raf.readInt();
			System.out.println(pageFile[i]);
		}
		for (int i = 0; i < pageFile.length; i++) {
			if (searchNumber == pageFile[i]) {
				System.out.println("Bre8hke to noumero " + searchNumber);
				raf.close();
				return numberOfAccesses;
			}

		}
		System.out.println("den bre8hke to noumero");
		raf.close();
		return numberOfAccesses;
	}

}
