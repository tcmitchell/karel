
{ karel maze demo: robot will follow the right wall until he finds a beeper }

BEGINNING-OF-PROGRAM
	DEFINE-NEW-INSTRUCTION turnright AS
		ITERATE 3 TIMES
			turnleft;

	BEGINNING-OF-EXECUTION
		WHILE not-next-to-a-beeper DO
			BEGIN
				IF right-is-clear
					THEN turnright
					ELSE
						WHILE front-is-blocked DO
							turnleft;
				move
			END;
		turnoff
	END-OF-EXECUTION

END-OF-PROGRAM
