/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.cpu.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.cpu.PE;
import ru.ispras.microtesk.model.cpu.TempVars;
import ru.ispras.microtesk.model.cpu.mode.*;

import static ru.ispras.microtesk.model.cpu.TypeDefs.*;

public final class JZ extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "JZ",
          JZ.class,
          null
          );
      addArgument("source", OPRNDL.INFO);
      addArgument("target", J_ADDR.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive source = (IsaPrimitive) getArgument("source", args);
      final IsaPrimitive target = (IsaPrimitive) getArgument("target", args);

      return new JZ(source, target);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive source;
  public final IsaPrimitive target;

  public JZ(final IsaPrimitive source, final IsaPrimitive target) {
    assert OPRNDL.INFO.isSupported(source);
    assert J_ADDR.INFO.isSupported(target);

    this.source = source;
    this.target = target;

    addArgument("source", source);
    addArgument("target", target);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("jz %s", target.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("11%s%s", source.image(vars__), target.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (source.access(pe__, vars__).load().equals(Data.valueOf(BYTE, 0x0))) {
      pe__.PC.access().store(target.access(pe__, vars__).load());
    } else {
      pe__.PC.access().store(pe__.PC.access().load().add(Data.valueOf(INDEX, 0x2)));
    }
  }
}