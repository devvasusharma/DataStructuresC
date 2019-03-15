//Lab1 work by Mahchehra Alimova

#include "stats.h"
#include <cassert>
namespace main_savitch_2C
{

        // CONSTRUCTOR
        statistician::statistician()
		{
        	count = 0;
        	total = 0;
        	//tinyest = 0;
        	//largest = 0;
        	myVec.clear();
		}
        // MODIFICATION MEMBER FUNCTIONS
        void statistician::next(double r)
        {
        	if (r > largest || count == 0) {largest = r;}
            if (tinyest > r || count == 0) {tinyest = r;}
            total += r;
        	myVec.push_back(r);
        	count++;
        }
        void statistician::reset()
        {
        	 count = 0;
        	 total = 0;
        	 //tinyest = 0;
        	 //largest = 0;
        	 myVec.clear();
        }
        // CONSTANT MEMBER FUNCTIONS
        int statistician::length() const
        {
        	return count;
        }
        double statistician::sum() const
        {
       		return total;
        }
        double statistician::mean() const
        {
        	assert(length() > 0);
        	return total/count;
        }
        double statistician::minimum() const
        {
        	assert(length() > 0);
        	return tinyest;
        }
        double statistician::maximum() const
        {
        	assert(length() > 0);
        	return largest;
        }
        double statistician::getItem(int i) const
        {
        	assert(length() > 0);
        	return myVec[i];
        }

        bool operator ==(const statistician& s1, const statistician& s2)
        {
        	if (s1.length() == 0 && s2.length() == 0){return true;}
        	if (s1.length() == 0 || s2.length() == 0){return false;}
        	return
        	(s1.length( ) == s2.length( ))
        	&&
        	(s1.sum( ) == s2.sum( ))
        	&&
        	(s1.minimum( ) == s2.minimum( ))
        	&&
        	(s1.maximum( ) == s2.maximum( ));
        }

        // FRIEND FUNCTIONS
        statistician operator +(const statistician& s1, const statistician& s2)
        {
        	if (s1.length() == 0){return s2;}
        	if (s2.length() == 0){return s1;}

        	statistician s3;
        	for(int i=0; i < s1.length(); i++)
        	{
        		s3.next(s1.getItem(i));
        	}
        	for(int i=0; i < s2.length(); i++)
        	{
        		s3.next(s2.getItem(i));
        	}
        	return s3;
        }
        statistician operator *(double scale, const statistician& s)
        {
        	double p;
        	statistician s1;

        	if (s.length() == 0){return s1;}

        	for(int i=0; i < s.length(); i++)
        	{
        		p = s.getItem(i)*scale;
        		s1.next(p);
        	}
        	return s1;
        }


}
