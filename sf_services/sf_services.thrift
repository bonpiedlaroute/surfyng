

struct Sf_struct
{
   1: string url
   2: string property_type
   3: i32 price
   4: i32 nb_rooms
   5: string description
   6: string images_path
}

service Sf_service
{
   list<Sf_struct> getproperties()
}
