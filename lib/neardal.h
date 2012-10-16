/*
 *     NEARDAL (Neard Abstraction Library)
 *
 *     Copyright 2012 Intel Corporation. All rights reserved.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License version 2
 *     as published by the Free Software Foundation.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software Foundation,
 *     Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
/*!
 * @file neardal.h
 *
 * @brief Defines main NEARDAL apis and data types.
 * @author Frederic PAUT, Intel Corporation
 * @version 0.1
 *
 ******************************************************************************/

#ifndef __NEARDAL_H
#define __NEARDAL_H
#include "neardal_errors.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */


/*!
 * @addtogroup NEARDAL_COMMON_TYPES Types
 * @ingroup NEARDAL_COMMON
 * @{
  */
/*!
 * @brief NEARDAL Adapter client properties.
 **/
typedef struct {
/*! \brief DBus interface adapter name (as identifier) */
	char	*name;
/*! \brief Neard adapter polling already active ? */
	short	polling;
/*! \brief Neard adapter powered ? */
	short	powered;
/*! \brief Number of supported protocols */
	int	nbProtocols;
/*! \brief Neard adapter supported protocols list (use  @link
 * neardal_free_array @endlink(& ) to free) */
	char	**protocols;
/*! \brief Number of tags managed by this adapter */
	int	nbTags;
/*! \brief Neard adapter tags list (use @link neardal_free_array @endlink
 *(& ) to free) */
	char	**tags;
} neardal_adapter;

/*!
 * @brief NEARDAL Tag client properties.
*/
typedef struct {
/*! @brief DBus interface tag name (as identifier) */
	const char	*name;

/*! @brief Number of records in tag */
	int		nbRecords;
/*! @brief tag records list (use @link neardal_free_array @endlink (& ) to
 * free) */
	char		**records;
/*! @brief Number of supported 'types' in tag */
	int		nbTagTypes;
/*! @brief types list (use @link neardal_free_array @endlink(& ) to free) */
	char		**tagType;
/*! @brief tag type */
	const char	*type;
/*! @brief Read-Only flag (is tag writable?) */
	short		readOnly;
} neardal_tag;

/*!
 * @brief NEARDAL Record client properties.
*/
typedef struct {
/*! @brief DBus interface name (as identifier) */
	const char	*name;
/*! @brief The character encoding */
	const char	*encoding;
/*! @brief The ISO/IANA language code (For example "en" or "jp") */
	const char	*language;
/*! @brief 'Action' Save, Edit, Download... */
	const char	*action;
/*! @brief The NDEF record type name ("SmartPoster", "Text", "URI",
 * "HandoverRequest", "HandoverSelect", "HandoverCarrier" */
	const char	*type;
/*! @brief human readable representation of a text or title record. */
	const char	*representation;
/*! @brief the complete URI, including the scheme and the resource */
	const char	*uri;
/*! @brief URI object size */
	unsigned int	uriObjSize;
/*! @brief The URI object MIME type */
	const char	*mime;
} neardal_record;

/* @}*/

/*! @brief NEARDAL Callbacks
 * @addtogroup NEARDAL_CALLBACK Callbacks
 * @{
*/

/**
 * @brief Callback prototype for 'NEARDAL adapter added/removed'
 *
 * @param adpName DBus interface adapter name (as identifier)
 * @param user_data Client user data
 **/
typedef void (*adapter_cb) (const char *adpName, void *user_data);
/**
 * @brief Callback prototype for 'NEARDAL adapter property changed'
 *
 * @param adpName DBus interface adapter name (as identifier)
 * @param propName Property name
 * @param value Property value
 * @param user_data Client user data
 **/
typedef void (*adapter_prop_cb) (char *adpName, char *propName, void *value,
				 void *user_data);

/** @brief NEARDAL Tag Callbacks (TagFound/Lost)
*/
/**
 * @brief Callback prototype for 'NEARDAL tag found/lost'
 *
 * @param tagName DBus interface tag name (as identifier)
 * @param user_data Client user data
 **/
typedef void (*tag_cb) (const char *tagName, void *user_data);

/** @brief NEARDAL Record Callbacks ('RecordFound')
*/
/**
 * @brief Callback prototype for 'NEARDAL record found'
 *
 * @param rcdName DBus interface record name (as identifier)
 * @param user_data Client user data
 **/
typedef void (*record_cb) (const char *rcdName, void *user_data);

/* @}*/



/*! @brief NEARDAL APIs
 *  @note NEARDAL lib exported functions
 * @addtogroup NEARDAL_APIS APIs
 * @{
*/

/*! \fn void neardal_destroy()
*  \brief destroy NEARDAL object instance, disconnect Neard Dbus connection,
* unregister Neard's events
*/
void neardal_destroy();

/*! @brief NEARDAL Properties identifiers
 * @addtogroup NEARDAL_CALLBACK Defines
 * @{ */
#define NEARD_ADP_MODE_INITIATOR		0
#define NEARD_ADP_MODE_TARGET			1
#define NEARD_ADP_MODE_DUAL			2


/* @}*/


/*! \fn errorCode_t neardal_start_poll_loop(char *adpName, int mode)
*  \brief Request Neard to start polling on specific NEARDAL adapter with
*  specific mode
*  \param adpName : DBus interface adapter name (as identifier)
*  \param mode : Polling mode (see NEARD_ADP_MODE_...)
*  @return errorCode_t error code
*/
errorCode_t neardal_start_poll_loop(char *adpName, int mode);

/*! \fn errorCode_t neardal_start_poll(char *adpName)
*  \brief Request Neard to start polling on specific NEARDAL adapter in
* Initiator mode
*  \param adpName : DBus interface adapter name (as identifier)
*  @return errorCode_t error code
*/
#define neardal_start_poll(adpName)	 neardal_start_poll_loop(adpName, \
						NEARD_ADP_MODE_INITIATOR);

/*! \fn errorCode_t neardal_stop_poll(char *adpName)
*  \brief Request Neard to stop polling on specific NEARDAL adapter
*  \param adpName : DBus interface adapter name (as identifier)
*  @return errorCode_t error code
*/
errorCode_t neardal_stop_poll(char *adpName);

/*! \fn errorCode_t neardal_get_adapters(char ***array, int *len)
 * @brief get an array of NEARDAL adapters present
 *
* @param array array of DBus interface adapter name (as identifier), use @link
 * neardal_free_array @endlink(& ) to free
 * @param len (optional), number of adapters
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_adapters(char ***array, int *len);

/*! \fn errorCode_t neardal_get_tags(char *adpName, char ***array, int *len)
 * @brief get an array of NEARDAL tags present
 *
 * @param adpName adapter name (identifier) on which tags list must be
 * retrieve
 * @param array array of DBus interface tag name (as identifier), use @link
 * neardal_free_array @endlink(& ) to free
 * @param len (optional), number of tags
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_tags(char *adpName, char ***array, int *len);

/*! \fn errorCode_t neardal_get_records(char *tagName, char ***array, int *len)
 * @brief get an array of NEARDAL records present
 *
 * @param tagName tag name (identifier) on which records list must be
 * retrieve
 * @param array array of DBus interface record name (as identifier), use @link
 * neardal_free_array @endlink(& ) to free
 * @param len (optional), number of records
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_records(char *tagName, char ***array, int *len);

/*! @fn errorCode_t neardal_free_array(char ***array)
 *
 * @brief free memory used for adapters array, tags array or records array
 *
 * @param array adapters array, tags array or records array
 * @return errorCode_t error code
 *
 **/
errorCode_t neardal_free_array(char ***array);

/*! @brief NEARDAL Properties identifiers
 * @addtogroup NEARDAL_CALLBACK Defines
 * @{ */
#define NEARD_ADP_PROP_POWERED			0


/* @}*/


/*! \fn errorCode_t neardal_get_adapter_properties(const char* adpName,
 * neardal_adapter **adapter)
 * @brief Get properties of a specific NEARDAL adapter
 *
 * @param adpName DBus interface adapter name (as identifier)
 * @param adapter Pointer on pointer of client adapter struct to store datas
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_adapter_properties(const char *adpName,
					   neardal_adapter **adapter);

/*! \fn void neardal_free_adapter(neardal_adapter * adapter)
 * @brief Release memory allocated for properties of an adapter
 *
 * @param adapter Pointer on client adapter struct where datas are stored
 * @return nothing
 **/
void neardal_free_adapter(neardal_adapter *adapter);

/*! \fn errorCode_t neardal_set_adapter_properties(const char* adpName,
 * int adpPropId, void * value)
 * @brief Set a property on a specific NEARDAL adapter
 *
 * @param adpName DBus interface adapter name (as identifier)
 * @param adpPropId Adapter Property Identifier (see NEARD_ADP_PROP_ ...)
 * @param value Value
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_adapter_property(const char *adpName,
					   int adpPropId, void *value);

/*! \fn errorCode_t neardal_set_cb_adapter_added( adapter_cb cb_adp_added,
 *					     void * user_data)
 * @brief setup a client callback for 'NEARDAL adapter added'. cb_adp_added = NULL
 * to remove actual callback
 *
 * @param cb_adp_added Client callback 'adapter added'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_adapter_added(adapter_cb cb_adp_added,
					 void *user_data);

/*! \fn errorCode_t neardal_set_cb_adapter_removed(adapter_cb cb_adp_removed,
 *					       void * user_data)
 * @brief setup a client callback for 'NEARDAL adapter removed'.
 * cb_adp_removed = NULL to remove actual callback
 *
 * @param cb_adp_removed Client callback 'adapter removed'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_adapter_removed(adapter_cb cb_adp_removed,
					    void *user_data);

/*! \fn errorCode_t neardal_set_cb_adapter_property_changed(
 * adapter_prop_cb cb_adp_property_changed,
 * void *user_data)
 * @brief setup a client callback for 'NEARDAL Adapter Property Changed'.
 * cb_adp_property_changed = NULL to remove actual callback.
 *
 * @param cb_adp_property_changed Client callback 'Adapter Property Changed'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_adapter_property_changed(
					adapter_prop_cb cb_adp_property_changed,
						void *user_data);

/*! \fn errorCode_t neardal_get_tag_properties(const char* tagName,
 * neardal_tag **tag)
 * @brief Get properties of a specific NEARDAL tag
 *
 * @param tagName tag name (identifier) on which properties must be retrieve
 * @param tag Pointer on pointer of client tag struct to store datas
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_tag_properties(const char *tagName,
					   neardal_tag **tag);

/*! \fn void neardal_free_tag(neardal_tag *tag)
 * @brief Release memory allocated for properties of a tag
 *
 * @param tag Pointer on client tag struct where datas are stored
 * @return nothing
 **/
void neardal_free_tag(neardal_tag *tag);

/*! \fn errorCode_t neardal_set_cb_tag_found(tag_cb cb_tag_found,
 * void * user_data)
 * @brief setup a client callback for 'NEARDAL tag found'.
 * cb_tag_found = NULL to remove actual callback.
 *
 * @param cb_tag_found Client callback 'tag found'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_tag_found(tag_cb cb_tag_found,
					 void *user_data);

/*! \fn errorCode_t neardal_set_cb_tag_lost(tag_cb cb_tag_lost,
 * void * user_data)
 * @brief setup a client callback for 'NEARDAL tag lost'.
 * cb_tag_lost = NULL to remove actual callback.
 *
 * @param cb_tag_lost Client callback 'tag lost'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_tag_lost(tag_cb cb_tag_lost,
					void *user_data);


/*! \fn errorCode_t neardal_get_record_properties(const char *recordName,
 *					      neardal_record **record)
 * @brief Get properties of a specific NEARDAL tag record
 *
 * @param recordName DBus interface record name (as identifier)
 * @param record Pointer on pointer of client record struct to store datas
 * @return errorCode_t error code
 **/
errorCode_t neardal_get_record_properties(const char *recordName,
					  neardal_record **record);

/*! \fn void neardal_free_record(neardal_record *record)
 * @brief Release memory allocated for properties of a record
 *
 * @param record Pointer on client tag struct where datas are stored
 * @return nothing
 **/
void neardal_free_record(neardal_record *record);

/*! \fn errorCode_t neardal_write(neardal_record *record)
 * @brief Write NDEF record to an NFC tag
 *
 * @param record Pointer on client record used to create NDEF record
 * @return errorCode_t error code
 **/
errorCode_t neardal_write(neardal_record *record);


/*! \fn errorCode_t neardal_set_cb_record_found( record_cb cb_rcd_found,
 * void * user_data)
 * @brief Setup a client callback for 'NEARDAL tag record found'.
 * cb_rcd_found = NULL to remove actual callback
 *
 * @param cb_rcd_found Client callback 'record found'
 * @param user_data Client user data
 * @return errorCode_t error code
 **/
errorCode_t neardal_set_cb_record_found(record_cb cb_rcd_found,
					 void *user_data);


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif /* __NEARDAL_H */