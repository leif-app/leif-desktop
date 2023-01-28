# leif Changelog

21.01.2023 V1.0.0.2 DS: - Added a QML warning logger for debugging purposes.
27.01.2023 V1.0.0.3 DS: - Added a generic logger. Added logging to the CarbonProcessor.
28.01.2023          DS: - Build system works now with shadow build on/off, debug and release.
           V1.0.0.4 DS: - Fixed an issue with the CarbonProcessor. It checks every minute now.
		                - Changed the CarbonData, it contains the forcast values now, next and later.
						- Refactores the UK plugin and the leif code to handle the new CarbonData structure.
						- Added caching to the CarbonProcessor to prohibit unneccassary data requiest from the API.
		                