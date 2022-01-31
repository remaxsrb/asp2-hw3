#include<iostream>
#include <fstream>
#include<Math.h>
#include<String>
#include <List>
#include<Windows.h>


using std::string;
void set_color(int i) 
{
	HANDLE hconsole;
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsole, i);
}
class AddressFunction // in case of colision
{
public:
	virtual int operator () ( int adr, int attempt, int size, int s) = 0;
};
class SplitSequenceLinearHasing : public AddressFunction
{

public:



	int operator ()(int adr, int attempt, int size, int s) override
	{

		adr = adr + s * attempt;
		while (adr > size)
		{
			//std::cout << "Out of scope, adr is: "<< adr << std::endl;
			adr -= (size / 2);
		}
		return adr;

		
	}

	
};

class Film 
{

private:
	 string original_language;
	string original_title;
	 long int revenue;
	 int runtime;
	struct release_date 
	{
		string year;
		string month;
		string day;
		
		release_date(string y, string m, string d) : year(y), month(m), day(d){}

	}release_date;

	void pisi(std::ostream& os) const 
	{
		os << original_language<<' '<<original_title << ' ' << revenue << ' '<<runtime <<' '<< release_date.year<< '-'<< release_date.month<<'-'<< release_date.day<<std::endl;
	}

public:

	Film(string og_language, string og_title, string y, string m, string d ,long int reven, int time) : 
		release_date(y, m, d), original_language(og_language), original_title(og_title), 
		revenue(reven), runtime(time){};
	friend std::ostream& operator<<(std::ostream& os, const Film& f) 
	{
		f.write(os);
		return os;
	}

	string get_title() { return original_title; }
	~Film(){};
};

struct Table 
{
	Film *f;
	int deleted;
};

class HashTable
{
private:
	int size;
	int count;
	int s1;
	int s2;
	int calculations;
	int found;
	int not_found;
	
	Table* table;
	



	void pisi(std::ostream& os) const
	{
		for (int i = 0; i < size; i++) 
		{
			if (table[i].f == nullptr)
				os  <<"EMPTY" << std::endl;
			else if (table[i].deleted == 1)
			{
				set_color(1);
				os  <<"DELETED" << std::endl;
				set_color(7);
			}
			else
				os  <<*table[i].f << std::endl;
		}
	}

public:
	friend std::ostream& operator<<(std::ostream& os, const HashTable& h)
	{
		h.pisi(os);
		return os;
	}

	HashTable(int size, int s1, int s2) : size(size), s1(s1), s2(s2),count(0), calculations(0), found(0), not_found(0)
	{
		table = new Table  [size];
		for (int i = 0; i < size; i++)
		{

			table[i].f = nullptr;
			table[i].deleted = 0;
		}
	
	} //jedini konstruktor

	

	int hash(string title, int s1, int s2) //Method which returns and index in hash table
	{
		
		if (title.compare("") == 0)
			return -1;

		int sum = 0;
		for (char &c : title)
		{
			sum += (c - '0'); //Converting strings to integers - calculating its ascii value as sum of all of ascii values of its chars
		}

		
		int primary_adr = (int)floor(pow(2, 7) * (fmod((0.61803 * sum), 1))); //primary hash
		//calculations++;
		if (primary_adr < 0)
		{
			
			return primary_adr;
			
		}
			

		

		if ((table[primary_adr].f!=nullptr)&&(((title.compare(table[primary_adr].f->get_title())) == 0)))
		{ 

			
				return primary_adr;
			
			
		}

		if (table[primary_adr].f == nullptr)
		{
			return primary_adr;
		}
			
		else //secondary hash
		{
			int adr = primary_adr;
		
	
			int attempt = 1;
			while (table[adr].f != nullptr )
			{
				
				if ((table[adr].f->get_title().compare(title) == 0)) 
				{
					
					//std::cout << " Already exists at address: " << adr << std::endl;
					return adr;
				}
					
				if (title > table[adr].f->get_title()) 
				{
					//std::cout << "second a access" << std::endl;
					adr = SplitSequenceLinearHasing()( adr, attempt, this->get_size(), s2);
					
				}
					
				else if (title < table[adr].f->get_title()) 
				{
					//std::cout << "second b access" << std::endl;
					adr = SplitSequenceLinearHasing()( adr, attempt, this->get_size(), s1);
					
				}
					
				
			}
			
			return adr;
		}
		
	}


	bool insert(Film* f)
	{
		if (count >= size)
			return false;
		int adr = hash((*f).get_title(), s1, s2);
		//std::cout << "Address returned: " << adr << " Movie at the address is: "<< (*f).get_title() <<std::endl;

		if ( adr<0)
			return false;

		if (table[adr].f == nullptr)
		{
			table[adr].f = f;
			//std::cout << (*table[adr]).get_title() << std::endl;;
			count++;
			return true;
		}
		else if (table[adr].f != nullptr && (((table[adr].f->get_title()).compare((*f).get_title())) != 0)) 
			return false;
		


	}


	Film* find_key(string key)
	{
		int h_x = hash(key, s1, s2);
		
		if (h_x > 0 && (table[h_x].f != nullptr) && (table[h_x].f->get_title() == key)) 
		{
			//std::cout << key << "Movie already located on address: " << h_x << std::endl << std::endl;
			return table[h_x].f;
		}
			
		else 
		{
			//std::cout << key << " je trazen na adresi: " << h_x << std::endl << std::endl;
			return nullptr;
		}
			
	}

	void clear() 
	{
		for (int i = 0; i < size; i++)
		{
			if (table[i].f != nullptr) 
			{
				delete_key(table[i].f->get_title());
				
			}
				
		}
	}

	int key_count() { return count; }
	int get_size() { return size; }

	double fill_ratio() { return count / (double)size; }

	double avg_Access_Succes() 
	{
		

		std::ifstream titles("movies_search_1.txt");
		if (titles.is_open())
		{
			std::cout << "File opened!" << std::endl;

			string title;

			while (!titles.eof())
			{
				std::getline(titles, title, '\n');
				if (find_key(title) != nullptr)  /*std::cout << std::endl;*/ 
				{
					found++;
					//std::cout << "Exists: " << *find_key(title) << std::endl;
				}
					
				else {}
					//std::cout << "Does not exist" << std::endl << std::endl;
				calculations++;
			}
		}
		else
			std::cout << "File not opened" << std::endl;
		titles.close();

		return (double)calculations / found;

	}
	double avg_Access_Unsuccess() 
	{
		std::ifstream titles("movies_search_1.txt");
		if (titles.is_open())
		{
			std::cout << "File opened!" << std::endl;

			string title;

			while (!titles.eof())
			{
				std::getline(titles, title, '\n');
				if (find_key(title) == nullptr )  /*std::cout << std::endl;*/
				{
					not_found++;
					//std::cout << "Exists: " << *find_key(title) << std::endl;
				}

				else {}
					//std::cout << "Does not exist" << std::endl << std::endl;
				calculations++;
			}
		}
		else
			std::cout << "File not opened" << std::endl;
		titles.close();

		return (double)calculations / not_found;
	}
	int avg_Access_Unsuccess_EST() {}
	void resetStatistics() { calculations = 0; found = 0; not_found = 0; };

	bool delete_key(string key) 
	{
		//Semi-free locations technique 
		if ((table[hash(key, s1, s2)].f) != nullptr && (table[hash(key, s1, s2)].deleted==0))
		{
			table[hash(key, s1, s2)].deleted = 1;
			count--;
			return true;
		}
		else
			return false;
		
	}

	~HashTable() 
	{
		
		delete[] table;
	}

	

};



void Test(HashTable &h, std::list <Film> &Films)
{
	std::list<Film> ::iterator i;
	for (i = Films.begin(); i != Films.end(); i++) 
	{
		if (h.insert(&(*i))) 
			std::cout<< "Count: "<< h.key_count() << std::endl;
		else 
			std::cout << "Unsuccessful insertion" << std::endl;
	}
	
	std::cout << "Average number of table access while successful searching: " << h.avg_Access_Succes() << std::endl;
	h.resetStatistics();
	std::cout << "Average number of table access while unsuccessful searching: " << h.avg_Access_Unsuccess() << std::endl;
	h.resetStatistics();


	
}

	
int main() 
{
	set_color(7);
	std::list <Film> Films;
	std::ifstream csvFilms("tmdb_movies.csv");
	int count = 0;
	if (csvFilms.is_open())

	{
		std::cout<<"Otvorio!" << std::endl;

		string lang;
		string title;
		string year;
		string month;
		string day;
		string rev;
		string duration;

		while (!csvFilms.eof())
		{

			std::getline(csvFilms, lang, ',');
			std::getline(csvFilms, title, ',');
			std::getline(csvFilms, year, '-');
			std::getline(csvFilms, month, '-');
			std::getline(csvFilms, day, ',');
			std::getline(csvFilms, rev, ',');
			std::getline(csvFilms, duration, '\n');

			if (count > 0)
			{

				int time;
				if (duration == "")
					time = -1;
				else
					time = std::stoi(duration);

				Film f(lang, title, year, month, day, std::stol(rev), time);
				Films.push_back(f);

			}
			count++;
		}
	}

	else
		std::cout  <<"File not opened!" << std::endl;

	csvFilms.close();



	HashTable h(6000, 30, 40);

	
	int choice;

	while (1) 
	{
		std::cout <<"=================MENU=================" << std::endl;
		std::cout  << "1. Test" << std:: endl;
		std::cout  << "2. Find key" << std::endl;
		std::cout  << "3. Insert Key" << std::endl;
		std::cout  << "4. Delete Key" << std::endl;
		std::cout   << "5. Clear table" << std::endl;
		std::cout  << "6. Number of keys" << std::endl;
		std::cout  << "7. Table size" << std::endl;
		std::cout  << "8. Fill ratio" << std::endl;

		std::cout  << "Choice: ";
		std::cin >> choice;

		switch (choice)
		{
		case 1:
			Test(h,Films);
			break;
		case 2:

		{
			
			if (h.find_key("Orgazmo") != nullptr)
				std::cout << "Exists: " << *h.find_key("Orgazmo") << std::endl;
			else
				std::cout  << "Does not exist" << std::endl;
			if (h.find_key("Spectre")!=nullptr)
				std::cout  << "Exists: " << *h.find_key("Spectre") << std::endl;
			else
				std::cout  << "Does not exist" << std::endl;
			if (h.find_key("Avatar") != nullptr)
				std::cout << "Postoji: " << *h.find_key("Avatar") << std::endl;
			else
				std::cout  << "Avatar does not exist" << std::endl;
		}
			break;
		case 3:
		{
			Film film("sr", "Kad porastem bicu kengur", "2004","3","25",10000,98);
			if (h.insert(&film)) 
			{
				if (h.find_key(film.get_title()))
				{
					std::cout << *h.find_key(film.get_title());
				}
				else
					std::cout  << "Not found!" << std::endl;
			}
			else
				std::cout  << "Unsucessful insertion" << std::endl;
		}
			break;
		case 4:
		{
			if (h.delete_key("Orgazmo"))
			{
				std::cout  << "Orgazmo is deleted" << std::endl;

			}
			else
				std::cout << "Movie is either deleted or does not exist!" << std::endl;
			if (h.delete_key("Rampage"))
			{
				std::cout  << "Rampage is delted" << std::endl;
				
			}
			else
				std::cout  << "Movie is either deleted or does not exist!" << std::endl;
			std::cout << h;
		}
			break;
		case 5:
			h.clear();
			if (h.key_count() == 0)
				std::cout  << "Table deleted" << std::endl;
			break;
		case 6:
			std::cout  << "Number of keys: " << h.key_count() << std::endl;
			break;
		case 7:
			std::cout  << "Table size: " << h.get_size() << std::endl;
			break;
		case 8:
			std::cout  << "Fill ratio: " << h.fill_ratio() << std::endl;
			break;

		default:
			Films.clear();
			exit(1);
			break;
		}

	}

	return 0;
}