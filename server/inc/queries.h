#ifndef QUERIES_H
#define QUERIES_H

#define GET_MESSAGES_QUERY "SELECT *\
							FROM (SELECT m.status, m.message_id, m.from_username, u.avatar,\
										 m.message_text, m.send_datetime, m.chat_id\
									FROM %s m\
									INNER JOIN %s u\
									ON u.username = m.from_username\
									WHERE m.chat_id=%s AND m.loaded_by NOT LIKE '%%%s%%'\
									ORDER BY m.message_id DESC)\
							ORDER BY message_id"

#define GET_CHATS_QUERY "  SELECT   c.status, \
                  c.chat_id, \
                  c.chat_name,\
                  CASE (c.chat_avatar) WHEN '?' THEN (SELECT avatar FROM %s u WHERE username=REPLACE(c.chat_members, '%s', '')) ELSE c.chat_avatar END CHAT_AVATAR, \
                  CASE MAX_MID IS NULL \
                      WHEN 1 THEN 'You' || ' @ ' || '...' || '@' || '...' \
                      ELSE (SELECT m3.from_username || '@' || m3.message_text || '@' || m3.status FROM %s m3 WHERE m3.message_id=MAX_MID) \
                  END MESSAGE_DATA \
                  , c.chat_members \
              FROM (SELECT *, (SELECT max(message_id) FROM %s m WHERE m.chat_id = c2.chat_id) AS MAX_MID FROM %s c2) c\
              WHERE c.chat_members LIKE '%%%s%%' AND c.loaded_by NOT LIKE '%%%s%%'"


//user queries
#define GET_USER_INFO_QUERY  "SELECT avatar FROM %s WHERE username='%s%s'"
#define UPDATE_USER_INFO_QUERY "UPDATE %s SET avatar='%s' WHERE username='%s'"
#define GO_ONLINE_QUERY "UPDATE %s SET online='online' WHERE username='%s'"
#define GO_OFFLINE_QUERY "UPDATE %s SET online='offline' WHERE username='%s'"

//messages queries
#define CREATE_MESSAGE_QUERY "INSERT INTO %s(from_username, message_text, send_datetime, chat_id) VALUES('%s', '%s', %s, %s)"
#define EDIT_MESSAGE_QUERY "UPDATE %s SET message_text='%s', loaded_by='', status='%s' WHERE message_id=%s"
#define DELETE_MESSAGE_QUERY "UPDATE %s SET loaded_by='', status='%s' WHERE message_id=%s"
#define CREATE_CHAT_QUERY "INSERT INTO %s VALUES(%d, '%s', '%s', '%s', '', '%s')"

//clean up queries
#define CLEANUP_DELETED_MESSAGES_QUERY "DELETE FROM %s WHERE status='%s' AND from_username='%s'"
#define CLEANUP_DELETED_CHATS_QUERY  "DELETE FROM %s WHERE status='%s' AND chat_members LIKE '%%%s%%'"

//status queries
#define RESET_LOADING_STATUS "UPDATE %s SET loaded_by=REPLACE(loaded_by, '%s', '') WHERE loaded_by LIKE '%%%s%%' AND status='%s'"
#define RESET_CHAT_MESSAGES_LOADING_STATUS  "UPDATE %s SET loaded_by=REPLACE(loaded_by, '%s', '') WHERE chat_id=%s"
#define UPDATE_MESSAGES_STATUS_QUERY "UPDATE %s AS m\
									  SET loaded_by= loaded_by || '%s',\
									  status=(CASE(status!='%s'\
											 AND\
											 LENGTH(m.loaded_by)+LENGTH(m.from_username)=LENGTH((SELECT chat_members FROM %s c WHERE c.chat_id=m.chat_id)))\
									  WHEN 1 THEN '%s' ELSE status END)\
									  WHERE message_id=%s"

#define UPDATE_CHAT_STATUS_QUERY "UPDATE %s SET loaded_by=loaded_by || '%s' WHERE chat_id=%s"
#endif

