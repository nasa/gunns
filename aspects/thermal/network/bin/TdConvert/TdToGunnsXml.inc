c======================================================
c Copyright 2019 United States Government as represented by the Administrator of the
c National Aeronautics and Space Administration.  All Rights Reserved.
c
c  This Fortran script is used to generate Gunns C++
c  ThermalNetwork output from a Thermal Desktop run.
c
c  Notes:
c        1. The Thermal Desktop model must build a
c           sub-model named 'SPACE'.
c        2. All user-defined radiation links (not
c           calculated by RADCAD) must be in a special
c           sub-model named 'RADIATION' or they will
c           be recognized as regular linear conductors.
c
c  Joe Valerioti, 2011
c
c======================================================

HEADER REGISTER DATA
      INT:NUSER11 = 0.
      INT:MAX_LINK_TOTAL = 10000.
      INT:DESIGNATES_CONDUCTION = 444
      INT:DESIGNATES_RADIATION = -111
      INT:flag_print = 0.
      INT:flag_space = 0.
      INT:i_sub_radcad = 0.
      INT:i_sub_radiation = 0.
      INT:i_sub = 0.
      INT:i_link = 0.
      INT:i_node = 0.
      INT:i_node_space = 0.
      INT:link_total = 0.
      INT:cond_max_per_sub = 0.
      INT:cond_ptr = 0.
      INT:cond_ptr_radcad = 0.
      INT:link_counter = 0.
      INT:designator = 0.
      INT:i_link_within_sub = 0.
      coeff = 0.0

HEADER ARRAY DATA, SPACE
      800 = ALLSAME, MAX_LINK_TOTAL, 0.0
      801 = ALLSAME, MAX_LINK_TOTAL, 0.0
      802 = ALLSAME, MAX_LINK_TOTAL, 0.0
      803 = ALLSAME, MAX_LINK_TOTAL, 0.0

HEADER CARRAY DATA, SPACE
C     file name for Network output data
      11 = TdNetworkConfig.xml

HEADER VARIABLES0, SPACE
C-----Open user files
      CALL USRFIL(nuser11, SPACE.UCA11)

C-----Find the total amount of conductors in the entire model
      do i_sub = 1, NMACT, 1
F         link_total = link_total + NCTOT(i_sub)

C---------Find the max number of conductors per submodel, so that we only have to loop to this amount later.
F         if (NCTOT(i_sub) .gt. cond_max_per_sub ) then
F             cond_max_per_sub = NCTOT(i_sub)
          endif

C---------If we come across a RADIATION submodel, then all the conductors within it will be radiation links.
F         if (NMOD(i_sub) .eq. 'RADIATION') then
              i_sub_radiation = i_sub
          endif
      enddo

C-----Warning issued if more conductors found than the size of the allocated arrays.
      if (link_total .gt. MAX_LINK_TOTAL) then
          write(nuser11,*) 'WARNING!!'
          write(nuser11,*) 'link_total, number of conductors in Thermal Desktop model is greater than'
          write(nuser11,*) 'MAX_LINK_TOTAL, the arbitrary size of the allocated conductor array.'
          write(nuser11,*) link_total,' = link_total'
          write(nuser11,*) MAX_LINK_TOTAL,' = MAX_LINK_TOTAL'
          write(nuser11,*) 'Increase the size of MAX_LINK_TOTAL in Thermal Desktop .inc file'
          write(nuser11,*)
          link_total = MAX_LINK_TOTAL
      endif

HEADER OUTPUT CALLS, SPACE
C-----Only print the post-run (steady state) data
      if (flag_print .eq. 0) then
           flag_print = 1
      else

C-----Print XML Header
      write(nuser11,1000)
      write(nuser11,1001)
C ______________________________________________________________________________________________________________________
C=====Node Printing

C-----Flag to specify when space node has been encountered
      flag_space = 0

C----Loop through all nodes
      do i_node = 1, NNOD ,1

C----Print every node except SPACE node
F        if(  TRIM(NDNAM(i_node)) .ne. 'SPACE' ) then

C------------Find temperature and capacitance of each node
F            ttest = T( INTNOD(NDNAM(i_node),NDINT(i_node)) )
F            ctest = C( INTNOD(NDNAM(i_node),NDINT(i_node)) )

C------------Print: node count, node submodel name, node integer, temperature, capacitance
             write(nuser11,1101)
F            write(nuser11,1011) 'name', TRIM(NDNAM(i_node)), NDINT(i_node), 'name'
             write(nuser11,1012) 'temperature', 'K',   ttest, 'temperature'
C------------Must use a different format whether the ctest >= 1.
             if (ctest .ge. 1) then
                 write(nuser11,1012) 'capacitance', 'J/K', ctest, 'capacitance'
             else
                 write(nuser11,1022) 'capacitance', 'J/K', ctest, 'capacitance'
             endif
             write(nuser11,1102)

C--------Save the index of the space node as i_node_space
         else
             flag_space = 1
             i_node_space = i_node
         endif
      enddo

C-----Write SPACE Node Last
F     ttest = T( INTNOD(NDNAM(i_node_space),NDINT(i_node_space)) )
F     ctest = C( INTNOD(NDNAM(i_node_space),NDINT(i_node_space)) )

C-----Print: final count, space node submodel name, space node integer, temperature, capacitance
      write(nuser11,1101)
F     write(nuser11,1011) 'name', TRIM(NDNAM(i_node_space)), NDINT(i_node_space), 'name'
      write(nuser11,1012) 'temperature', 'K',   ttest, 'temperature'
      write(nuser11,1022) 'capacitance', 'J/K', ctest, 'capacitance'
      write(nuser11,1102)

C ______________________________________________________________________________________________________________________
C-----Find the index of the RADCAD submodel
      do i_sub = 2, NMACT ,1

C---------Submodels in NMOD are listed alphabetically - SINDA submodels first, then RADCAD submodels.
          if(  ICHAR( NMOD(i_sub) ) .lt. ICHAR( NMOD(i_sub-1) ) ) then
              i_sub_radcad = i_sub
              cond_ptr_radcad = INTCON(NMOD(i_sub_radcad), -1)
          endif
      enddo

C-----Loop through all models
      do i_sub = 1, NMACT ,1

C---------Loop through all links. Unfortunately, the indices of the NMOD array do not line up with
C---------the indices of the NCTOT array. To be safe, we loop through the max # of conductors per submodel.
          do i_link_within_sub = 1, cond_max_per_sub, 1

C-------------Get conductor pointer
              cond_ptr = INTCON(NMOD(i_sub), -1 * i_link_within_sub)

C-------------If the pointer is positive, and has not be recorded yet,...
              if ( (cond_ptr .gt. 0) .AND. (A(800+cond_ptr) .eq. 0) ) then

C-----------------Store the submodel and link indices into their respective arrays
                  A(801+cond_ptr) = i_sub
                  A(802+cond_ptr) = i_link_within_sub
F                 A(803+cond_ptr) = G(cond_ptr)

C-----------------The conductor is a radiation link if its submodel is 'RADIATION' or in the RADCAD group.
                  if ( (i_sub .eq. i_sub_radiation) .OR. (cond_ptr .ge. cond_ptr_radcad) ) then
                      A(800+cond_ptr) = DESIGNATES_RADIATION
                  else
                      A(800+cond_ptr) = DESIGNATES_CONDUCTION
                  endif
              endif

          enddo
      enddo

C ______________________________________________________________________________________________________________________
C==== Radiation Printing

C-----Loop through the link array
      do i_link = 1, link_total, 1

          designator = A(800+i_link)

C---------A negative designator flags a radiation link
          if (designator .eq. DESIGNATES_RADIATION) then

C-------------Get the indices previously stored.
              i_sub = A(801+i_link)
              i_link_within_sub = A(802+i_link)
F             coeff = A(803+i_link)

C-------------Error check
              if ( (i_sub .eq. 0) .OR. (i_link_within_sub .eq. 0) ) then
                  write(nuser11,*) 'Warning! CONDAT call to index 0.'
              else
C-----------------Get the link info and print.
F                 CALL CONDAT( NMOD(i_sub), i_link_within_sub, node0, node1, qtest)

                  if (coeff .ge. 0.001) then
                      write(nuser11,1301)
F                     write(nuser11,1011) 'node0', TRIM(NDNAM(node0)), NDINT(node0), 'node0'
F                     write(nuser11,1011) 'node1', TRIM(NDNAM(node1)), NDINT(node1), 'node1'
C---------------------Must use a different format whether the coeff >= 1.
                      if (coeff .ge. 1) then
                          write(nuser11,1013) 'coefficient', 'm2', coeff, 'coefficient'
                      else
                          write(nuser11,1023) 'coefficient', 'm2', coeff, 'coefficient'
                      endif
                      write(nuser11,1302)

C---------------------Increment the counter
                      link_counter = link_counter + 1
                  endif

              endif
          endif
      enddo

C ______________________________________________________________________________________________________________________
C==== Conduction Printing

C-----Reset the link counter
      link_counter = 0

C-----Loop through the link array
      do i_link = 1, link_total, 1

          designator = A(800+i_link)

C---------A positive designator flags a conduction link
          if (designator .eq. DESIGNATES_CONDUCTION) then

C-------------Get the indices stored previously.
              i_sub = A(801+i_link)
              i_link_within_sub = A(802+i_link)
F             coeff = A(803+i_link)

C-------------Error check
              if ( (i_sub .eq. 0) .OR. (i_link_within_sub .eq. 0) ) then
                  write(nuser11,*) 'Warning! CONDAT call to index 0.'
              else

C-----------------Get the link info and print.
F                 CALL CONDAT( NMOD(i_sub), i_link_within_sub, node0, node1, qtest)

                  if (coeff .ge. 0.001) then
                      write(nuser11,1201)
F                     write(nuser11,1011) 'node0', TRIM(NDNAM(node0)), NDINT(node0), 'node0'
F                     write(nuser11,1011) 'node1', TRIM(NDNAM(node1)), NDINT(node1), 'node1'
C---------------------Must use a different format whether the coeff >= 1.
                      if (coeff .ge. 1) then
                          write(nuser11,1013) 'conductivity', 'W/K', coeff, 'conductivity'
                      else
                          write(nuser11,1023) 'conductivity', 'W/K', coeff, 'conductivity'
                      endif
                      write(nuser11,1202)

C---------------------Increment the counter
                      link_counter = link_counter + 1
                  endif

              endif
          endif
      enddo


C ______________________________________________________________________________________________________________________
C-----Close the <list>, clear variables
      write(nuser11,1002)
      mtest = 0
      ntest = 0
      qtest = 0
      ttest = 0
      ctest = 0
      endif

C---------Formats used for printing
 1000 format('<?xml version="1.0" ?>')
 1001 format('<network>')
 1002 format('</network>')
 1011 format('        <',A,'>', A, '_', I0, '</',A,'>')
 1012 format('        <',A,' units="',A,'">', F0.2, '</',A,'>')
 1013 format('        <',A,' units="',A,'">', F0.6, '</',A,'>')
 1022 format('        <',A,' units="',A,'">', F4.2, '</',A,'>')
 1023 format('        <',A,' units="',A,'">', F8.6, '</',A,'>')
 1101 format('    <node>')
 1102 format('    </node>')
 1201 format('    <conduction>')
 1202 format('    </conduction>')
 1301 format('    <radiation>')
 1302 format('    </radiation>')
