// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// This file is part of PaInleSS.
//
// PaInleSS is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#pragma once

#include "../clauses/ClauseExchange.h"
#include "../simplify/simplify.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

#define ID_SYM 0
#define ID_XOR 1

struct parameter {
   int tier1;
   int chrono;
   int stable;
   int walkinitially;
   int target;
   int phase;
   int heuristic;
   int margin;
   int ccanr;
   int targetinc;
};


/// Code for SAT result
enum SatResult
{
	SAT     = 10,
	UNSAT   = 20,
	UNKNOWN = 0
};


/// Code  for the type of solvers
enum SolverType
{
	GLUCOSE   = 0,
	LINGELING = 1,
	MAPLE     = 2,
	MINISAT   = 3
};


/// Structure for solver statistics
struct SolvingStatistics
{ 
   /// Constructor
	SolvingStatistics()
   {
      propagations = 0;
      decisions    = 0;
      conflicts    = 0;
      restarts     = 0;
      memPeak      = 0;
   }

	unsigned long propagations; ///< Number of propagations.
	unsigned long decisions;    ///< Number of decisions taken.
	unsigned long conflicts;    ///< Number of reached conflicts.
	unsigned long restarts;     ///< Number of restarts.
	double        memPeak;      ///< Maximum memory used in Ko.
};


/// Interface of a solver that provides standard features.
class SolverInterface
{
public:
   /// Load formula from a given dimacs file, return false if failed.
   virtual void addOriginClauses(simplify *S) = 0;
   
   virtual void setBumpVar(int v) = 0;

   virtual bool loadFormula(const char* filename) = 0;
   virtual bool loadFormula(FILE* tempfile)=0;

   /// Get the number of variables of the current resolution.
   virtual int getVariablesCount() = 0;

   /// Get a variable suitable for search splitting.
   virtual int getDivisionVariable() = 0;

   /// Set initial phase for a given variable.
   virtual void setPhase(const int var, const bool phase) = 0;

   /// Bump activity of a given variable.
   virtual void bumpVariableActivity(const int var, const int times) = 0;

   /// Interrupt resolution, solving cannot continue until interrupt is unset.
   virtual void setSolverInterrupt() = 0;

   /// Remove the SAT solving interrupt request.
   virtual void unsetSolverInterrupt() = 0;

   /// Solve the formula with a given cube.
   virtual SatResult solve(const std::vector<int> & cube) = 0;

   /// Add a permanent clause to the formula.
   virtual void addClause(ClauseExchange * clause) = 0;

   /// Add a list of permanent clauses to the formula.
   virtual void addClauses(const std::vector<ClauseExchange *> & clauses) = 0;

   /// Add a list of initial clauses to the formula.
   virtual void addInitialClauses(const std::vector<ClauseExchange *> & clauses) = 0;

   /// Add a learned clause to the formula.
   virtual void addLearnedClause(ClauseExchange * clauses) = 0;
   
   /// Add a list of learned clauses to the formula.
   virtual void addLearnedClauses(const std::vector<ClauseExchange *> & clauses) = 0;

   /// Get a list of learned clauses.
   virtual void getLearnedClauses(std::vector<ClauseExchange *> & clauses) = 0;

   /// Request the solver to produce more clauses.
   virtual void increaseClauseProduction() = 0;
   
   /// Request the solver to produce less clauses.
   virtual void decreaseClauseProduction() = 0;

   /// Get solver statistics.
   virtual SolvingStatistics getStatistics() = 0;

   /// Return the model in case of SAT result.
   virtual std::vector<int> getModel() = 0;

   /// Native diversification.
   virtual void diversify(int id) = 0;

   virtual void setParameter(parameter p) = 0;

   virtual void initshuffle(int id) = 0;

   /// Return the final analysis in case of UNSAT result.
   virtual std::vector<int> getFinalAnalysis() = 0;

   virtual std::vector<int> getSatAssumptions() = 0;


   /// Constructor.
   SolverInterface(int solverId, SolverType solverType)
   {
      id    = solverId;
      type  = solverType;
      nRefs = 1;
   }

   /// Destructor.
   virtual ~SolverInterface()
   {
   }

   /// Increase the counter of references of this solver.
   void increase()
   {
      nRefs++;
   }

   /// Decrease the counter of references of this solver, delete it if needed.
   void release()
   {
      int oldValue = nRefs.fetch_sub(1);

      if (oldValue - 1 == 0) {
         delete this;
      }
   }

   /// Id of this solver.
   int id;

   /// Type of this solver.
   SolverType type;

   /// Number of references pointing on this solver.
   atomic<int> nRefs;
};
