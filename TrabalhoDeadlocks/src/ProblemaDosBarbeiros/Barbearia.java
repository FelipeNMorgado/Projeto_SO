package ProblemaDosBarbeiros;

public class Barbearia {
	
	// Operação chamada pelos clientes
	
	public boolean cortaCabelo(Cliente c) {
		// Barbearia não lotada: espera que o corte seja feito + return true
		// Barbearia lotada: return false
		return false;
	}
	
	
	// Operações chamadas pelos barbeiros
	
	public Cliente proximoCliente() {
		// Seleciona o próximo cliente (o barbeiro pode dormir esperando o cliente)
	
		return null;
	}
	
	public void corteTerminado(Cliente c) {
		// Barbeiro acorda o cliente que está na sua cadeira e espera que ele saia da barbearia
	}

}
