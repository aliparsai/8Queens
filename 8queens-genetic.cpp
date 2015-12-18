#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>



using namespace std;

long long int checked_positions;

/* creates a 64-bit random */

unsigned long long int
random_64 (void)
{
  unsigned long long int result = 0;
  int i;

  for (i = 0; i < 8; i++)
    result = (result * 256) + (rand () % 256);

  return result;
}



/* compares two numbers, used in qsort */
int
compare (const void *a, const void *b)
{
  return (((*(int *) a - *(int *) b) > 0) ? 1 : -1);
}

/* checks if an integer is not already in the array */
int
check_unique (int num, int *array, int length)
{
  int result = 1, i;
  if (length < 1)
    return 1;
  for (i = 0; i < length; i++)
    if (array[i] == num)
      {
	result = 0;
	break;
      }
  return result;
}

/* fills an integer array */

int
fill_the_array (int *array, int length, int max)
{
  int i, tmp;

  if (length == 0)
    return 0;

  for (i = 0; i < length; i++)
    {
      do
	{
	  tmp = rand () % max;
	}
      while (!check_unique (tmp, array, i));

      array[i] = tmp;
    }

  qsort (array, length, sizeof (int), compare);
  return 0;

}


/* calculates absolute value of a number */
int
absolute (int num)
{
  return ((num < 0) ? (-num) : (num));
}

/* probability function */
int
probability (int desired, int max)
{
  int random_number = rand () % max;
  if (random_number < desired)
    return 1;
  return 0;
}

// ---------------------------------------------------------//

/* a class for an organism */
class organism
{
public:
  unsigned char *q;
  int fitness;

/* overriding operator = */
    organism & operator= (const organism & rhs)
  {
    if (this == &rhs)
      return *this;

    int i;
      fitness = rhs.fitness;
    for (i = 0; i < 8; i++)
        q[i] = rhs.q[i];

      return *this;
  }

/* constructor */
  organism ()
  {
    int i;
    q = new unsigned char[8];

/* initialize randomly */
    for (i = 0; i < 8; i++)
      q[i] = rand () % 8;
  }


/* calculates fitness by calculating the sum of differences of evaluation to the evaluation of expert in all positions */
  void calculate_fitness (void)
  {
    int sum = 0, i, j;
    for (i = 0; i < 8; i++)
      for (j = i + 1; j < 8; j++)
	if (q[i] == q[j] || q[i] + i == q[j] + j || q[i] - i == q[j] - j)
	  sum++;

    fitness = 29 - sum;
    checked_positions++;
  }

  void show ()
  {
    cout << endl;
    int i, j;
    for (i = 0; i < 8; i++)
      {
	cout << "\n   +----+----+----+----+----+----+----+----+\n" << 8 -
	  i << "  ";
	for (j = 0; j < 8; j++)
	  cout << (j == q[i] ? "| Q  " : "|    ");
	cout << "|";
      }
    cout << "\n   +----+----+----+----+----+----+----+----+\n";
    cout << "      a    b    c    d    e    f    g    h\n";
  }

};


// ---------------------------------------------------------//
class generation
{
public:
  organism * gene;
  organism *best;
  int num;
  int index;

  /* constructor */
    generation ()
  {
    gene = new organism[1500];
    num = 1500;
    index = 0;
  }

  /* mutates an organism */
  int mutation (organism * x)
  {
    int r = rand () % 8;
    x->q[r] = rand () % 8;

    return r;
  }


  int crossover (organism parent1, organism parent2, organism * child)
  {
    int i, r = rand () % 8;

    for (i = 0; i < 8; i++)
      if (i < r)
	child->q[i] = parent1.q[i];
      else
	child->q[i] = parent2.q[i];
    return 0;
  }

};


/***********************************************************

selection function to create a new generation from an old 
generation. this is the most important function which deals
with genetic algorithm performance.

***********************************************************/
void
selection (generation * oldg, generation * newg, fstream * log)
{
  int in[70], i, j, i1, j1;

  int sum = 0, average, upper_bound = 0, lower_bound =
    29, filled_organisms = 0, bound, *prob, *qprob, sumofprob =
    0, first, second, mutated = 0, crossover1 = 0, crossover2 = 0, direct = 0;

  cout << endl << "Generation: " << oldg->index << endl;
  *log << endl << "Generation: " << oldg->index << endl;

  prob = new int[oldg->num];
  qprob = new int[oldg->num];

  for (i = 0; i < oldg->num; i++)
    {
/* initialize fitness values for all the organisms */
      oldg->gene[i].calculate_fitness ();
      cout << "\rCalculating fitness values: " << i << "/" << oldg->num;
      cout.flush ();
/* calculate sum of all fitness values */
      sum += oldg->gene[i].fitness;

/* calculate upper and lower bounds of fitness values */
      if (upper_bound < oldg->gene[i].fitness)
	upper_bound = oldg->gene[i].fitness;
      if (lower_bound > oldg->gene[i].fitness)
	lower_bound = oldg->gene[i].fitness;
    }
/* calculate average of all fitness values */
  average = sum / oldg->num;

  if (sum < 2)
    sum = 2000;
  for (i = 0; i < oldg->num; i++)
    prob[i] = oldg->gene[i].fitness * 100000 / sum;


  for (i = 0; i < oldg->num; i++)
    {
      qprob[i] = 0;
      for (j = 0; j <= i; j++)
	qprob[i] += prob[j];
    }


/* newg is the next generation */
  newg->index = oldg->index + 1;

  cout << " done. Average fitness: " << average << "  Best Organism: " <<
    upper_bound << endl;

  *log << "Average fitness: " << average << "  Best Organism: " << upper_bound
    << endl;

/* create organisms for new generation while it is not filled */

  for (i = 0; i < oldg->num; i++)
    if (oldg->gene[i].fitness == upper_bound)
      {
//      newg->gene[filled_organisms] = oldg->gene[i];
	oldg->best = &(oldg->gene[i]);

	*log << endl;
	for (i1 = 0; i1 < 8; i1++)
	  *log << (int) oldg->best->q[i1] << " , ";
	*log << endl;

	//filled_organisms++;
	//direct++;
	break;
      }

  while (filled_organisms < oldg->num - 1)
    {
//     if(oldg->index%40==0 && oldg->index < 300 && filled_organisms >oldg->num/2) break; // adding random immigrants each 40 generations.

      cout << "\rFilling Organisms: " << filled_organisms << "/" << oldg->num
	<< "  d:" << direct << " c1:" << crossover1 << " c2:" << crossover2 <<
	" m:" << mutated;


      first = random_64 () % qprob[oldg->num - 1];
      second = random_64 () % qprob[oldg->num - 1];

      for (i = 1; i < oldg->num - 1; i++)
	if (qprob[i - 1] < first && qprob[i] > first)
	  break;

      if (first < qprob[0])
	i = 0;

      for (j = 1; j < oldg->num - 1; j++)
	if (qprob[j - 1] < second && qprob[j] > second)
	  break;

      if (second < qprob[0])
	j = 0;

/* Crossover */
      if (probability (75, 100))

	if (filled_organisms < oldg->num - 5)
	  {
	    newg->crossover (oldg->gene[i], oldg->gene[j],
			     &(newg->gene[filled_organisms]));
	    filled_organisms += 1;
	    crossover2++;
	  }

	else
	  {
	    newg->gene[filled_organisms] = oldg->gene[i];
	    filled_organisms++;
	    direct++;
	  }



/* Mutation */

      if (probability (40, 10000))
	{
	  i = rand () % oldg->num;
	  newg->gene[filled_organisms] = oldg->gene[i];
	  newg->mutation (&(newg->gene[filled_organisms]));
	  filled_organisms++;
	  mutated++;
	}
    }

  cout << "\rFilling Organisms: " << filled_organisms << "/" << oldg->num
    << "  d:" << direct << " c1:" << crossover1 << " c2:" << crossover2 <<
    " m:" << mutated;

}

/**********************************************************/

// ---------------------------------------------------------//

int
main (int argc, char *argv[])
{
/* initializing random seed */
  srand (time (0));
  checked_positions = 0;
  int i = 0;
  generation ga[1000];
  fstream input_file, output_file;

  output_file.open ("ga.log", fstream::out);



  cout <<
    "\n\nGenetic alghorithm solver for 8 queens problem.\nDesigned by Ali Parsai.\n";

  for (i = 0; i < 999; i++)
    {
      selection (&ga[i], &ga[i + 1], &output_file);
      if (ga[i].best->fitness == 29)
	{
	  cout << "\nfound the answer. checked positions: " <<
	    checked_positions;
	  break;
	}
    }

  ga[i].best->show ();

  cout << "\n\n";
  return 0;
}
