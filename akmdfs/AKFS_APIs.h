/******************************************************************************
 *
 * Copyright (C) 2012 Asahi Kasei Microdevices Corporation, Japan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef AKFS_INC_APIS_H
#define AKFS_INC_APIS_H

/* Include files for AK8975 library. */
#include "AKFS_Compass.h"

/*** Constant definition ******************************************************/
#define AKFS_GEOMAG_MAX 70

/*** Type declaration *********************************************************/

/*** Global variables *********************************************************/

/*** Prototype of function ****************************************************/
int16 AKFS_Init(
			void		*mem,
	const	AKFS_PATNO	hpat,
	const	uint8		regs[]
);

void AKFS_Release(void *mem);

int16 AKFS_Start(void *mem, const char *path);

int16 AKFS_Stop(void *mem, const char *path);

int16 AKFS_Get_MAGNETIC_FIELD(
			void		*mem,
	const   int16		mag[3],
	const	int16		status,
			AKFLOAT		*hx,
			AKFLOAT		*hy,
			AKFLOAT		*hz,
			int16		*accuracy
);

int16 AKFS_Get_ACCELEROMETER(
			void		*mem,
	const   int16		acc[3],
	const	int16		status,
			AKFLOAT		*ax,
			AKFLOAT		*ay,
			AKFLOAT		*az,
			int16		*accuracy
);

int16 AKFS_Get_ORIENTATION(
			void		*mem,
			AKFLOAT		*azimuth,
			AKFLOAT		*pitch,
			AKFLOAT		*roll,
			int16		*accuracy
);

#endif

