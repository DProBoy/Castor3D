#include "Smaa_Parsers.hpp"

#include "SmaaPostEffect.hpp"

#include <Engine.hpp>
#include <Event/Frame/FrameListener.hpp>
#include <Material/Material.hpp>
#include <Overlay/BorderPanelOverlay.hpp>
#include <Render/RenderTarget.hpp>
#include <Scene/SceneFileParser.hpp>

#include <stack>

namespace smaa
{
	struct ParserContext
	{
		castor3d::Engine * engine{ nullptr };
		Preset preset;
		SmaaConfig::Data data;
	};

	ParserContext & getParserContext( castor::FileParserContextSPtr context )
	{
		return *static_cast< ParserContext * >( context->getUserContext( PostEffect::Type ) );
	}

	IMPLEMENT_ATTRIBUTE_PARSER( parserSmaa )
	{
		ParserContext * context = new ParserContext;
		context->engine = std::static_pointer_cast< castor3d::SceneFileContext >( p_context )->m_pParser->getEngine();
		p_context->registerUserContext( PostEffect::Type, context );
	}
	END_ATTRIBUTE_PUSH( SmaaSection::eRoot )

	IMPLEMENT_ATTRIBUTE_PARSER( parserMode )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			uint32_t value{ 0u };
			p_params[0]->get( value );
			context.data.mode = Mode( value );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserPreset )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			uint32_t value{ 0u };
			p_params[0]->get( value );
			context.preset = Preset( value );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserEdgeDetection )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			uint32_t value{ 0u };
			p_params[0]->get( value );
			context.data.edgeDetection = EdgeDetectionType( value );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserDisableDiagonalDetection )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.disableDiagonalDetection );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserDisableCornerDetection )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.disableCornerDetection );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserThreshold )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.threshold );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserMaxSearchSteps )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.maxSearchSteps );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserMaxSearchStepsDiag )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.maxSearchStepsDiag );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserCornerRounding )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.cornerRounding );
		}
	}
	END_ATTRIBUTE()
		
	IMPLEMENT_ATTRIBUTE_PARSER( parserPredication )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.enablePredication );
		}
	}
	END_ATTRIBUTE()
		
	IMPLEMENT_ATTRIBUTE_PARSER( parserReprojection )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.enableReprojection );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserReprojectionWeightScale )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.reprojectionWeightScale );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserLocalContrastAdaptationFactor )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.localContrastAdaptationFactor );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserPredicationScale )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.predicationScale );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserPredicationStrength )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.predicationStrength );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserPredicationThreshold )
	{
		auto & context = getParserContext( p_context );

		if ( p_params.empty() )
		{
			PARSING_ERROR( "Missing parameter" );
		}
		else
		{
			p_params[0]->get( context.data.predicationThreshold );
		}
	}
	END_ATTRIBUTE()

	IMPLEMENT_ATTRIBUTE_PARSER( parserSmaaEnd )
	{
		auto & context = getParserContext( p_context );
		auto engine = context.engine;
		auto parsingContext = std::static_pointer_cast< castor3d::SceneFileContext >( p_context );
		castor3d::Parameters parameters;
		parameters.add( cuT( "mode" ), getName( context.data.mode ) );
		parameters.add( cuT( "preset" ), getName( context.preset ) );
		parameters.add( cuT( "edgeDetection" ), getName( context.data.edgeDetection ) );
		parameters.add( cuT( "disableDiagonalDetection" ), context.data.disableDiagonalDetection );
		parameters.add( cuT( "disableCornerDetection" ), context.data.disableCornerDetection );
		parameters.add( cuT( "localContrastAdaptationFactor" ), context.data.localContrastAdaptationFactor );
		parameters.add( cuT( "enablePredication" ), context.data.enablePredication );
		parameters.add( cuT( "predicationScale" ), context.data.predicationScale );
		parameters.add( cuT( "predicationStrength" ), context.data.predicationStrength );
		parameters.add( cuT( "predicationThreshold" ), context.data.predicationThreshold );

		if ( context.preset == Preset::eCustom )
		{
			parameters.add( cuT( "threshold" ), context.data.threshold );
			parameters.add( cuT( "maxSearchSteps" ), context.data.maxSearchSteps );
			parameters.add( cuT( "maxSearchStepsDiag" ), context.data.maxSearchStepsDiag );
			parameters.add( cuT( "cornerRounding" ), context.data.cornerRounding );
		}

		if ( context.data.mode == Mode::eT2X )
		{
			parameters.add( cuT( "enableReprojection" ), context.data.enableReprojection );
			parameters.add( cuT( "reprojectionWeightScale" ), context.data.reprojectionWeightScale );
		}

		auto effect = engine->getRenderTargetCache().getPostEffectFactory().create( PostEffect::Type
			, *parsingContext->renderTarget
			, *engine->getRenderSystem()
			, parameters );
		parsingContext->renderTarget->addPostEffect( effect );

		delete reinterpret_cast< ParserContext * >( p_context->unregisterUserContext( PostEffect::Type ) );
	}
	END_ATTRIBUTE_POP()
}
