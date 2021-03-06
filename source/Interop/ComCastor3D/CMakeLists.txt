if ( MSVC )
	project( ComCastor3D )
	include( ComUtils )
	find_package( ATL )

	if ( ATL_FOUND )
		message( STATUS "+ Found ATL" )
		set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
		set( ${PROJECT_NAME}_VERSION_MINOR 11 )
		set( ${PROJECT_NAME}_VERSION_BUILD	0 )

		configure_file( 
			${CMAKE_CURRENT_SOURCE_DIR}/Src/Win32/ComCastorUtils.hpp.in
			${CMAKE_CURRENT_BINARY_DIR}/Src/ComCastorUtils.hpp
			NEWLINE_STYLE LF
		)

		include_directories( ${Castor3DIncludeDirs} )
		include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src )
		include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src/Castor3D )
		include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src/CastorUtils )
		include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
		include_directories( ${CMAKE_CURRENT_BINARY_DIR}/Src )
		include_directories( ${CMAKE_CURRENT_BINARY_DIR}/Src/Win32 )
		include_directories( ${ATL_INCLUDE_DIR} )

		#ATL Libs
		set( AtlLibraries "" )
		FOREACH( Lib ${ATL_LIBRARIES} )
			if ( AtlLibraries )
				set( AtlLibraries "${AtlLibraries}|${Lib}" )
			else ()
				set( AtlLibraries "${Lib}" )
			endif ()
		ENDFOREACH()

		# Define Interfaces and their RGS files
		set( RESOURCE_ID 102 )
		set( RESRC_LIST "" )
		set( RESH_LIST "" )
		# CastorUtils interfaces
		add_interface( "2C6A4321-6187-436B-9B31-4B340D1C6507" RESRC_LIST RESH_LIST RESOURCE_ID "Position" )
		add_interface( "3F1B36E8-4E97-403C-B2F3-25267C23A7FD" RESRC_LIST RESH_LIST RESOURCE_ID "Size" )
		add_interface( "A1C8B71D-D5BC-4664-9058-1EE9F964BFF7" RESRC_LIST RESH_LIST RESOURCE_ID "Rect" )
		add_interface( "9A7B169B-06DF-4617-A93D-3EF10A4362F7" RESRC_LIST RESH_LIST RESOURCE_ID "Vector2D" )
		add_interface( "D406D2D9-865B-40CF-AB98-CA79F00C0797" RESRC_LIST RESH_LIST RESOURCE_ID "Vector3D" )
		add_interface( "EF3A4D20-DC85-493C-BBC3-0A17065CF17A" RESRC_LIST RESH_LIST RESOURCE_ID "Vector4D" )
		add_interface( "22125089-C34A-4C20-B8F9-07907A8AEA0D" RESRC_LIST RESH_LIST RESOURCE_ID "RgbaColour" )
		add_interface( "7D790705-5D16-4D1E-A557-957D6D4E01E7" RESRC_LIST RESH_LIST RESOURCE_ID "RgbColour" )
		add_interface( "A25F52A6-5CF2-45F5-BED8-FB0294FCA339" RESRC_LIST RESH_LIST RESOURCE_ID "HdrRgbaColour" )
		add_interface( "097DF712-996C-4E4B-89FB-9C0D06F42BE7" RESRC_LIST RESH_LIST RESOURCE_ID "HdrRgbColour" )
		add_interface( "236D0939-4F04-4D7C-A8CD-1415F3CCBA71" RESRC_LIST RESH_LIST RESOURCE_ID "Angle" )
		add_interface( "7A301C67-36E2-483D-925A-62E3C5DBE871" RESRC_LIST RESH_LIST RESOURCE_ID "Quaternion" )
		add_interface( "3353741E-3F82-4A99-B8DC-6D6AB0584E04" RESRC_LIST RESH_LIST RESOURCE_ID "Matrix4x4" )
		add_interface( "E8EB965B-EB5E-413D-B119-FADC081C8AD9" RESRC_LIST RESH_LIST RESOURCE_ID "Glyph" )
		add_interface( "814FF4F4-8FFC-4048-A972-6EF3DA5314E1" RESRC_LIST RESH_LIST RESOURCE_ID "Font" )
		add_interface( "D2F8DD56-82D0-4467-9AB4-61F3BB228ACC" RESRC_LIST RESH_LIST RESOURCE_ID "PixelBuffer" )
		add_interface( "032154A9-2E6F-4FEE-A132-5840EEEC21FD" RESRC_LIST RESH_LIST RESOURCE_ID "Image" )
		add_interface( "A39A1B4B-9882-4B2E-AB1D-55257AF40727" RESRC_LIST RESH_LIST RESOURCE_ID "Logger" )

		# Castor3D interfaces
		add_interface( "557E0DD9-00E5-48A2-AE26-B78CC6031956" RESRC_LIST RESH_LIST RESOURCE_ID "Engine" )
		add_interface( "E59360F1-2F5B-4011-A61F-32E1A77207C8" RESRC_LIST RESH_LIST RESOURCE_ID "Submesh" )
		add_interface( "C5053069-A376-4E0E-AA0A-5CCE3A78C205" RESRC_LIST RESH_LIST RESOURCE_ID "Mesh" )
		add_interface( "C36B1CB1-9623-4DE9-B5D8-108AC6D32D54" RESRC_LIST RESH_LIST RESOURCE_ID "MovableObject" )
		add_interface( "9C77B39B-EAD2-4EDB-815E-1B74B1F8B5C3" RESRC_LIST RESH_LIST RESOURCE_ID "LightCategory" )
		add_interface( "F0FC01FC-2C13-40C0-A07F-9B913181264B" RESRC_LIST RESH_LIST RESOURCE_ID "DirectionalLight" )
		add_interface( "3581BE19-66C5-407B-8E7E-C761424C5974" RESRC_LIST RESH_LIST RESOURCE_ID "PointLight" )
		add_interface( "0819BA84-8671-454B-B9A3-84C8A3C5DB95" RESRC_LIST RESH_LIST RESOURCE_ID "SpotLight" )
		add_interface( "823F16E0-606C-46C7-8B22-8B5C907B4E48" RESRC_LIST RESH_LIST RESOURCE_ID "Camera" )
		add_interface( "7499D2D6-4275-4816-B25E-042CC75C566E" RESRC_LIST RESH_LIST RESOURCE_ID "Light" )
		add_interface( "0F08578A-FD57-4F6F-8CB5-26E5C1CBA88A" RESRC_LIST RESH_LIST RESOURCE_ID "Geometry" )
		add_interface( "665CE022-156D-4C04-B621-35CB66A6D6D4" RESRC_LIST RESH_LIST RESOURCE_ID "SceneNode" )
		add_interface( "0A9CE856-F576-41EF-A6F0-C27291C6C2D2" RESRC_LIST RESH_LIST RESOURCE_ID "OverlayCategory" )
		add_interface( "B5AD045F-0EDE-46F9-8AF0-381FBDB62385" RESRC_LIST RESH_LIST RESOURCE_ID "PanelOverlay" )
		add_interface( "147C4638-20C2-46AC-8765-B67F0128BC4A" RESRC_LIST RESH_LIST RESOURCE_ID "BorderPanelOverlay" )
		add_interface( "287EAF30-2165-49AC-BC96-8CF568D28CAE" RESRC_LIST RESH_LIST RESOURCE_ID "TextOverlay" )
		add_interface( "CA66D170-B96A-4910-83D5-D622E9A813F7" RESRC_LIST RESH_LIST RESOURCE_ID "Overlay" )
		add_interface( "F9185125-9460-4039-A0B0-EB2295AC6AB1" RESRC_LIST RESH_LIST RESOURCE_ID "Animable" )
		add_interface( "F31A778F-F9E5-4619-A35B-2EE8325BF1D2" RESRC_LIST RESH_LIST RESOURCE_ID "AnimatedObject" )
		add_interface( "7180F914-17E1-45A1-A2AB-F195F0724AD7" RESRC_LIST RESH_LIST RESOURCE_ID "AnimatedObjectGroup" )
		add_interface( "C2669704-BF96-4D28-9627-7E625EA2EC99" RESRC_LIST RESH_LIST RESOURCE_ID "Scene" )
		add_interface( "5B26DE5C-FFE8-42AE-A07C-837D5164CAFE" RESRC_LIST RESH_LIST RESOURCE_ID "Animation" )
		add_interface( "53A552B5-8524-4DDD-987C-AC68AA87233E" RESRC_LIST RESH_LIST RESOURCE_ID "Skeleton" )
		add_interface( "AD82930D-4682-4CF1-B9E8-EDFD869E7AF7" RESRC_LIST RESH_LIST RESOURCE_ID "RenderTarget" )
		add_interface( "68E4373D-24BB-45FE-9717-CB512111A984" RESRC_LIST RESH_LIST RESOURCE_ID "RenderWindow" )
		add_interface( "CA7E6B72-3130-4888-83D3-33BFBBA2B3FD" RESRC_LIST RESH_LIST RESOURCE_ID "BlendState" )
		add_interface( "8699BD20-FDE7-4FA9-9265-60B539CCE45D" RESRC_LIST RESH_LIST RESOURCE_ID "DepthStencilState" )
		add_interface( "3720BBC7-6B1D-454B-A062-677C4E2B365E" RESRC_LIST RESH_LIST RESOURCE_ID "MultisampleState" )
		add_interface( "5880CD0C-969C-471D-8FD0-3F271A492D33" RESRC_LIST RESH_LIST RESOURCE_ID "RasteriserState" )
		add_interface( "98978C6B-C6FB-447C-A874-15068F900011" RESRC_LIST RESH_LIST RESOURCE_ID "Sampler" )
		add_interface( "3CA68187-BD17-4E78-8D5C-B994DDA134BA" RESRC_LIST RESH_LIST RESOURCE_ID "TextureImage" )
		add_interface( "0FF08968-9937-4771-8949-0E86E3615F98" RESRC_LIST RESH_LIST RESOURCE_ID "TextureLayout" )
		add_interface( "E8DD6F29-F296-4687-AADD-F6FD1D0AC670" RESRC_LIST RESH_LIST RESOURCE_ID "TextureUnit" )
		add_interface( "B62FE384-D806-4DA7-B091-A7E260D95D7F" RESRC_LIST RESH_LIST RESOURCE_ID "ShaderProgram" )
		add_interface( "1B6D6829-C5D7-4B75-8A92-E016C63147B3" RESRC_LIST RESH_LIST RESOURCE_ID "Pass" )
		add_interface( "2A4B5235-063A-454B-860C-CA91ED042E2D" RESRC_LIST RESH_LIST RESOURCE_ID "LegacyPass" )
		add_interface( "20C67D15-677C-43DE-98B5-E328293740AF" RESRC_LIST RESH_LIST RESOURCE_ID "MetallicRoughnessPbrPass" )
		add_interface( "5ED10C5B-248B-427A-818F-C4F94A5FA54F" RESRC_LIST RESH_LIST RESOURCE_ID "SpecularGlossinessPbrPass" )
		add_interface( "A9F3E2A5-A54C-4E4A-8448-0A663BE88E40" RESRC_LIST RESH_LIST RESOURCE_ID "Material" )

		# Add RGS files to the sources
		file( GLOB COMCASTOR3D_RGS_FILES
			${CMAKE_CURRENT_BINARY_DIR}/Src/Win32/*.rgs
			${CMAKE_CURRENT_SOURCE_DIR}/Src/Win32/*.rgs
		)
		source_group( "Registry Files" FILES ${COMCASTOR3D_RGS_FILES} )

		# Update resources IDs
		configure_file( 
			${CMAKE_CURRENT_SOURCE_DIR}/Src/Win32/resource.h.in
			${CMAKE_CURRENT_BINARY_DIR}/Src/Win32/resource.h
			NEWLINE_STYLE LF
		)

		# Define IDL files
		file( GLOB COMCASTOR3D_IDL_FILES Src/*.idl )
		file( GLOB COMCASTOR3D_DEF_FILES_RELEASE Src/${PROJECT_NAME}.def )
		file( GLOB COMCASTOR3D_DEF_FILES_DEBUG Src/${PROJECT_NAME}d.def )

		if ( MSVC )
		  set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEF:${COMCASTOR3D_DEF_FILES_DEBUG}")
		  set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEF:${COMCASTOR3D_DEF_FILES_RELEASE}")
		  set( CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /DEF:${COMCASTOR3D_DEF_FILES_RELEASE}")
		endif()

		# MIDL compiler
		add_com_interfaces( COMCASTOR3D_SOURCES_IDL_FILES ${COMCASTOR3D_IDL_FILES} )
		set( COMCASTOR3D_ADDITIONAL_FILES
			${COMCASTOR3D_IDL_FILES}
			${COMCASTOR3D_RGS_FILES}
			${COMCASTOR3D_SOURCES_IDL_FILES}
		)
		source_group( "IDL Files" FILES ${COMCASTOR3D_IDL_FILES} )
		source_group( "IDL Files" FILES ${COMCASTOR3D_SOURCES_IDL_FILES} )
		source_group( "IDL Files" FILES ${COMCASTOR3D_DEF_FILES} )

		add_target( 
			${PROJECT_NAME}
			dll
			"CastorUtils;Castor3D"
			"CastorUtils;Castor3D;${AtlLibraries}"
			"ComCastor3DPch.hpp"
			"ComCastor3DPch.cpp"
			""
			""
			""
			"${CMAKE_CURRENT_BINARY_DIR}/Src/ComCastorUtils.hpp;${COMCASTOR3D_ADDITIONAL_FILES}"
		)

		file( GLOB COMCASTOR_HEADER_FILES
			Src/CastorUtils/*.hpp
			Src/CastorUtils/*.h
			Src/CastorUtils/*.inl
		)
		file( GLOB COMCASTOR_SOURCE_FILES
			Src/CastorUtils/*.cpp
			Src/CastorUtils/*.c
		)
		source_group( "Header Files\\CastorUtils" FILES ${COMCASTOR_HEADER_FILES} )
		source_group( "Source Files\\CastorUtils" FILES ${COMCASTOR_SOURCE_FILES} )
		
		file( GLOB COMCASTOR3D_HEADER_FILES
			Src/Castor3D/*.hpp
			Src/Castor3D/*.h
			Src/Castor3D/*.inl
		)
		file( GLOB COMCASTOR3D_SOURCE_FILES
			Src/Castor3D/*.cpp
			Src/Castor3D/*.c
		)
		source_group( "Source Files\\Castor3D" FILES ${COMCASTOR3D_SOURCE_FILES} )
		source_group( "Header Files\\Castor3D" FILES ${COMCASTOR3D_HEADER_FILES} )

		# Copy tlb files into binaries directory.
		add_custom_command( TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_BINARY_DIR}/Castor3D.tlb ${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin/
		)

		# install tlb files
		install(
			FILES ${CMAKE_CURRENT_BINARY_DIR}/Castor3D.tlb
			DESTINATION bin
			COMPONENT ${PROJECT_NAME}
		)

		register_target( ${PROJECT_NAME} )

		set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Interop" )
		add_target_astyle( ${PROJECT_NAME} ".hpp;.inl;.cpp" )
		set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
	else ()
		set( Build "no (ATL libraries not found)" PARENT_SCOPE )
	endif ()
endif ()
