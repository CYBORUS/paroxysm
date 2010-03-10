/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MATHENGINE_H
#define MATHENGINE_H

#include <boost/random.hpp>

class MathEngine
{
    public:
        template<class T> static T supremeRandom(T min, T max);

    private:
        static boost::mt19937 mSeed;
};

template<class T>
T MathEngine::supremeRandom(T min, T max)
{
    boost::uniform_real<T> u(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<T> >
        outValue(mSeed, u);
    return outValue();
}

#endif
