template <typename Type> 
Type Math::absolute(Type value){
	return (value < 0) ? -value : value;
}

template <typename Type> 
char Math::sign(Type value){
	if (value == 0) return 0;
	else if (value < 0) return -1;
	else return 1;
}


template <typename Type> 
Type Math::maxof(Type a, Type b){
	return (a > b) ? a : b;
}
		
		
template <typename Type> 
Type Math::minof(Type a, Type b){
	return (a < b) ? a : b;
}